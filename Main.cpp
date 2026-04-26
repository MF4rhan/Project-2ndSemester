// locations for the system("cls"), replace it with system("clear") for linux.
// Main menu loop: Line 177
// Case 5 (Fleet status): Line 424
// Case 6 (Complete shipment): Line 449
// Case 7 (Assign crate): Line 511
//
// Also, you will see these two lines often:
// cin.ignore(1000, '\n');
// cin.get();
//
// Because these two allow the program to pause at that point, waiting for any user input
// I couldn't find any other universal way to pause (for both windows and linux)
// system("pause") would only work for windows and not for linux, which was a issue for me
// so i found that these two will serve the same function

#include "Assets.h"
#include "Logistics.h"
#include "Registry.h"
#include "DataManager.h"
#include "Exceptions.h"

// this custom function below centers text on the screen for better menu formatting
void printC(string message, int width = 140)
{
    int padding = (width - (int)message.length()) / 2;
    cout << string(padding, ' ') << message;
}

// initialize the static order counter for shipments
int ShipmentOrder::orderCount = 0;

// this is a helper to print all vehicles and let the user pick one by index
TransportAsset* pickVehicle(const Registry<TransportAsset*>& fleet)
{
    if (fleet.empty())
    {
        cout << "\033[1;31mNo vehicles in fleet. Add one first (option 8).\033[0m\n";
        return nullptr;
    }

    printC("\033[1;34m==== Fleet ====\033[0m\n");
    for (int i = 0; i < fleet.size(); i++)
    {
        cout << i + 1 << ". " << fleet[i]->getName() << " (" << fleet[i]->getTypeName() << ") - ID: " << fleet[i]->getAssetID() << "\n";
        cout << "   Load: " << fleet[i]->getCurrentLoad() << "/" << fleet[i]->getMaxPayLoad() << "kg, Status: " << fleet[i]->getStatus() << "\n";
    }

    cout << "Select vehicle (1-" << fleet.size() << "): ";
    int ch;
    while (true)
    {
        cin >> ch;
        if (ch >= 1 && ch <= fleet.size())
        {
            break;
        }
        cout << "Invalid. Try again: ";
    }
    return fleet[ch - 1];
}

// this handles the long process of asking for all vehicle stats and creating the object
void addVehicle(Registry<TransportAsset*>& fleet)
{
    static int nextID = 1; // simple auto increment for vehicle IDs

   printC("\033[1;34m==== ADD VEHICLE ====\033[0m\n");
    cout << "Vehicle type:\n";
    cout << "1. Ground Transport (truck)\n";
    cout << "2. Air Transport (plane)\n";
    cout << "3. Water Transport (ship)\n";
    cout << "4. Heavy Lift Drone\n";
    cout << "Select: ";
    int type;
    while (true)
    {
        cin >> type;
        if (type >= 1 && type <= 4)
        {
            break;
        }
        cout << "Invalid. Try again: ";
    }

    string name, status, depot;
    double maxLoad;

    cout << "Vehicle name: "; cin >> name;
    cout << "Max payload (kg): "; cin >> maxLoad;
    cout << "Home depot/base: "; cin >> depot;
    cout << "Status (e.g. working / maintenance / retired): "; cin >> status;

    int id = nextID++;
    TransportAsset* v = nullptr;

    // build the specific derived class based on user choice
    if (type == 1)
    {
        string plate, roadLevel;
        double speed, fuel;
        cout << "License plate: "; cin >> plate;
        cout << "Speed (km/h): "; cin >> speed;
        cout << "Road clearance level: "; cin >> roadLevel;
        cout << "Fuel level (0-100): "; cin >> fuel;
        v = new GroundTransport(id, maxLoad, 0, name, status, depot, plate, speed, roadLevel, fuel);
    }
    else if (type == 2)
    {
        string tail;
        double alt, speed;
        int range;
        bool runway;
        cout << "Tail number: "; cin >> tail;
        cout << "Cruise altitude (m): "; cin >> alt;
        cout << "Air speed (km/h): "; cin >> speed;
        cout << "Max flight range (km): "; cin >> range;
        cout << "Requires runway? (1/0): "; cin >> runway;
        v = new AirTransport(id, maxLoad, 0, name, status, depot, tail, alt, speed, range, runway);
    }
    else if (type == 3)
    {
        string regNum, vesselType, portReg;
        double knots, draught;
        int crew;
        bool ocean;
        cout << "Registration number: "; cin >> regNum;
        cout << "Speed (knots): "; cin >> knots;
        cout << "Vessel type: "; cin >> vesselType;
        cout << "Draught (m): "; cin >> draught;
        cout << "Max crew capacity: "; cin >> crew;
        cout << "Port of registry: "; cin >> portReg;
        cout << "Ocean-going? (1/0): "; cin >> ocean;
        v = new WaterTransport(id, maxLoad, 0, name, status, depot, regNum, knots, vesselType, draught, crew, portReg, ocean);
    }
    else
    {
        string plate, tail, model, roadLevel;
        double gSpeed, fuel, alt, aSpeed, battery, urbanSpeed, aerialSpeed;
        int range, hover;
        bool runway, autonomous;
        cout << "License plate: "; cin >> plate;
        cout << "Ground speed (km/h): "; cin >> gSpeed;
        cout << "Road clearance level: "; cin >> roadLevel;
        cout << "Fuel level (0-100): "; cin >> fuel;
        cout << "Tail number: "; cin >> tail;
        cout << "Cruise altitude (m): "; cin >> alt;
        cout << "Air speed (km/h): "; cin >> aSpeed;
        cout << "Max flight range (km): "; cin >> range;
        cout << "Requires runway? (1/0): "; cin >> runway;
        cout << "Drone model: "; cin >> model;
        cout << "Battery level: "; cin >> battery;
        cout << "Autonomous? (1/0): "; cin >> autonomous;
        cout << "Max hover time (min): "; cin >> hover;
        cout << "Urban speed: "; cin >> urbanSpeed;
        cout << "Aerial speed: "; cin >> aerialSpeed;
        v = new HeavyLiftDrone(id, maxLoad, 0, name, status, depot, plate, gSpeed, roadLevel, fuel, tail, alt, aSpeed, range, runway, model, battery, autonomous, hover, urbanSpeed, aerialSpeed);
    }

    fleet.add(v);
    cout << "\033[1;32mAdded " << name << " to fleet. ID is " << id << "\033[0m\n";
}

int main()
{
    // core data structures
    Registry<CargoCrate*> crates;
    Registry<ShipmentOrder*> shipments;
    Registry<TransportAsset*> fleet;

    DataManager dataManager;
    CustomsAuditor auditor(1, "Muhammad Farhan", "GULF", "", 20);

    int userChoice = 1;

    do
    {
        system("cls");
        // system("clear");

        if (userChoice < 1 || userChoice > 11)
        {
            printC("\033[1;31m--- Incorrect input, please try again ---\033[0m\n");
        }

        // print the main menu
        printC("\033[1;34m===== SUPPLY CHAIN LOGISTICS =====\033[0m\n");
        printC("1.  Create new shipment\n");
        printC("2.  Load crate onto vehicle\n");
        printC("3.  Dispatch shipment\n");
        printC("4.  Run customs audit\n");
        printC("5.  View fleet status\n");
        printC("6.  Complete shipment\n");
        printC("7.  Assign crate to shipment\n");
        printC("8.  Add vehicle to fleet\n");
        printC("9.  Save data to file\n");
        printC("10. Load data from file\n");
        printC("\033[1;34m11. Exit\033[0m\n");
        cin >> userChoice;

        switch (userChoice)
        {
            case 1: // create a new shipment order
            {
                printC("\033[1;34m==== CREATE SHIPMENT ====\033[0m\n");
                string origin, destination, priorityLevel, status, cargoType;
                double totalWeight;
                bool international;
                int ch;

                cout << "Enter origin location: "; cin >> origin;
                cout << "Enter destination: "; cin >> destination;
                cout << "Enter total weight (kg): "; cin >> totalWeight;

                cout << "Priority:\n1. STANDARD\n2. EXPRESS\n3. CRITICAL\nSelect: ";
                cin >> ch;
                switch (ch)
                {
                    case 2: priorityLevel = "EXPRESS"; break;
                    case 3: priorityLevel = "CRITICAL"; break;
                    default: priorityLevel = "STANDARD"; break;
                }

                cout << "Cargo type:\n1. GENERAL\n2. PERISHABLE\n3. HAZARDOUS\nSelect: ";
                cin >> ch;
                switch (ch)
                {
                    case 2: cargoType = "PERISHABLE"; break;
                    case 3: cargoType = "HAZARDOUS"; break;
                    default: cargoType = "GENERAL"; break;
                }

                cout << "International? (1=Yes, 0=No): ";
                cin >> ch;
                international = (ch == 1);

                // ask user to assign this shipment to a vehicle immediately
                cout << "\nAssign to vehicle:\n";
                TransportAsset* assigned = pickVehicle(fleet);
                int assignedID = (assigned != nullptr) ? assigned->getAssetID() : -1;

                shipments.add(new ShipmentOrder(origin, destination, totalWeight, priorityLevel, "PENDING", cargoType, international, assignedID));
                cout << "\033[1;32mShipment created.\033[0m\n";
                cin.ignore(1000, '\n');
                cin.get();
                break;
            }

                    case 2: // create a crate and load it onto a vehicle
                    {
                        printC("\033[1;34m==== LOAD CRATE ====\033[0m\n");
                        cout << "\nSelect vehicle to load crate onto:\n";
                        TransportAsset* vehicle = pickVehicle(fleet);
                        if (vehicle == nullptr)
                        {
                            cin.ignore(1000, '\n');
                            cin.get();
                            break;
                        }

                        int tempID;
                        string tempDesc, tempFrag, tempHam, tempCountry;
                        double tempWeight;

                        cout << "Crate ID: "; cin >> tempID;
                        cout << "Contents description: "; cin >> tempDesc;
                        cout << "Weight (kg): "; cin >> tempWeight;
                        cout << "Fragility (FRAGILE / STANDARD): "; cin >> tempFrag;
                        cout << "Hazmat code: "; cin >> tempHam;
                        cout << "Origin country: "; cin >> tempCountry;

                        CargoCrate* newCrate = new CargoCrate(tempID, tempWeight, tempDesc, tempFrag, tempHam, tempCountry);

                        // Try to load it, catch our custom exceptions if rules are broken
                        try
                        {
                            vehicle->loadCrate(*newCrate);
                            cout << "\033[1;32mCrate loaded onto " << vehicle->getName() << ".\033[0m\n";
                        }
                        catch (const LogisticsException& e)
                        {
                            cout << "\033[1;31m" << e.getMessage() << "\n";
                            cout << "Crate NOT loaded onto vehicle.\033[0m\n";
                        }

                        crates.add(newCrate);
                        cin.ignore(1000, '\n');
                        cin.get();
                        break;
                    }

                    case 3: // dispatch an existing shipment
                    {
                        printC("\033[1;34m==== DISPATCH SHIPMENT ====\033[0m\n");
                        if (shipments.empty())
                        {
                           cout << "\033[1;31mNo shipments to dispatch.\033[0m\n";
                            cin.ignore(1000, '\n');
                            cin.get();
                            break;
                        }

                        cout << "\n=== Shipments ===\n";
                        for (int i = 0; i < shipments.size(); i++)
                        {
                            cout << "Order ID: " << shipments[i]->getOrderID() << ", Priority: " << shipments[i]->getPriority() << ", Status: " << shipments[i]->getStatus() << "\n";
                        }

                        cout << "Enter Order ID to dispatch: ";
                        int tempID;
                        cin >> tempID;

                        ShipmentOrder* target = nullptr;
                        for (int i = 0; i < shipments.size(); i++)
                        {
                            if (shipments[i]->getOrderID() == tempID)
                            {
                                target = shipments[i];
                                break;
                            }
                        }

                        if (target == nullptr)
                        {
                            cout << "\033[1;31mOrder ID not found.\033[0m\n";
                            cin.ignore(1000, '\n');
                            cin.get();
                            break;
                        }
                        if (target->getStatus() == "IN_TRANSIT" || target->getStatus() == "DELIVERED")
                        {
                           cout << "\033[1;31mShipment is already dispatched or delivered.\033[0m\n";
                            cin.ignore(1000, '\n');
                            cin.get();
                            break;
                        }

                        // assign transport and calculate time
                        cout << "\nSelect vehicle to dispatch with:\n";
                        TransportAsset* vehicle = pickVehicle(fleet);
                        if (vehicle == nullptr)
                        {
                            cin.ignore(1000, '\n');
                            cin.get();
                            break;
                        }

                        cout << "Distance to destination (km): ";
                        double dist;
                        cin >> dist;

                        double transitTime = vehicle->calculateTransitTime(dist);
                        target->setStatus("IN_TRANSIT");

                        cout << "\033[1;32mShipment " << tempID << " dispatched on " << vehicle->getName() << ".\033[0m\n";
                        if (transitTime > 0)
                        {
                            cout << "Estimated transit time: " << transitTime << " hrs\n";
                        }
                        else
                        {
                            cout << "Transit time unavailable (speed not set).\n";
                        }

                        cin.ignore(1000, '\n');
                        cin.get();
                        break;
                    }

                    case 4: // Customs audit for international orders
                    {
                        printC("\033[1;34m==== CUSTOMS AUDIT ====\033[0m\n");
                        if (shipments.empty())
                        {
                            cout << "No shipments.\n";
                            cin.ignore(1000, '\n');
                            cin.get();
                            break;
                        }

                        cout << "\n=== Shipments ===\n";
                        for (int i = 0; i < shipments.size(); i++)
                        {
                            cout << "Order ID: " << shipments[i]->getOrderID() << ", International: " << (shipments[i]->getIsInternational() ? "Yes" : "No") << ", Status: " << shipments[i]->getStatus() << "\n";
                        }

                        int ID;
                        bool flag = false;
                        while (!flag)
                        {
                            cout << "Enter Order ID to audit (0 to cancel): ";
                            cin >> ID;
                            if (ID == 0)
                            {
                                break;
                            }

                            for (int i = 0; i < shipments.size(); i++)
                            {
                                if (shipments[i]->getOrderID() == ID)
                                {
                                    flag = true;
                                    if (!shipments[i]->getIsInternational())
                                    {
                                        cout << "Domestic shipment - no customs audit required.\n";
                                    }
                                    else
                                    {
                                        auditor.auditShipment(*shipments[i]);
                                    }
                                }
                            }
                            if (!flag)
                            {
                               cout << "\033[1;31mID not found, try again.\033[0m\n";
                            }
                        }
                        cin.ignore(1000, '\n');
                        cin.get();
                        break;
                    }

                    case 5: // Show fleet status and global dashboard
                    {
                        system("cls");
                        // system("clear");

                        printC("\033[1;34m==== FLEET STATUS ====\033[0m\n");
                        if (fleet.empty())
                        {
                            cout << "No vehicles in fleet.\n";
                        }
                        else
                        {
                            for (int i = 0; i < fleet.size(); i++)
                            {
                                cout << i + 1 << ". " << fleet[i]->getName() << " (" << fleet[i]->getTypeName() << ") - ID: " << fleet[i]->getAssetID() << "\n";
                                cout << "   Load: " << fleet[i]->getCurrentLoad() << "/" << fleet[i]->getMaxPayLoad() << "kg, Status: " << fleet[i]->getStatus() << "\n";
                            }
                        }
                        printC("\033[1;34m======================\033[0m\n");
                        ShipmentOrder::renderGlobalDashboard();
                        cin.ignore(1000, '\n');
                        cin.get();
                        break;
                    }

                    case 6: // Mark a shipment as delivered and free up its vehicle
                    {
                        system("cls");
                        // system("clear");

                        printC("\033[1;34m==== ACTIVE SHIPMENTS ====\033[0m\n");
                        bool anyActive = false;
                        for (int i = 0; i < shipments.size(); i++)
                        {
                            if (shipments[i]->getStatus() != "DELIVERED")
                            {
                                cout << "ID: " << shipments[i]->getOrderID() << " | " << shipments[i]->getOriginNode() << " -> " << shipments[i]->getDestinationNode() << "\n";
                                cout << "    Status: " << shipments[i]->getStatus() << ", Priority: " << shipments[i]->getPriority() << "\n";
                                anyActive = true;
                            }
                        }

                        if (!anyActive)
                        {
                            cout << "\033[1;31mNo active shipments.\033[0m\n";
                            cin.ignore(1000, '\n');
                            cin.get();
                            break;
                        }

                        cout << "\nEnter Order ID to mark as delivered: ";
                        int tempID;
                        cin >> tempID;

                        bool found = false;
                        for (int i = 0; i < shipments.size(); i++)
                        {
                            if (shipments[i]->getOrderID() == tempID)
                            {
                                found = true;
                                shipments[i]->setStatus("DELIVERED");

                                // Find the vehicle that was carrying it and reset its load
                                for (int j = 0; j < fleet.size(); j++)
                                {
                                    if (fleet[j]->getAssetID() == shipments[i]->getAssignedAssetID())
                                    {
                                        fleet[j]->resetLoad();
                                        break;
                                    }
                                }

                                cout << "\033[1;32mShipment " << tempID << " delivered.\033[0m\n";
                                ShipmentOrder::renderGlobalDashboard();
                                break;
                            }
                        }

                        if (!found)
                        {
                            cout << "\033[1;31mOrder ID not found.\033[0m\n";
                        }
                        cin.ignore(1000, '\n');
                        cin.get();
                        break;
                    }

                    case 7: // Link a specific crate to a shipment order
                    {
                        system("cls");
                        // system("clear");

                        if (crates.empty() || shipments.empty())
                        {
                            cout << "\033[1;31mNeed both crates and shipments available.\033[0m\n";
                            cin.ignore(1000, '\n');
                            cin.get();
                            break;
                        }

                        printC("\033[1;34m========== CRATES ==========\033[0m\n");
                        for (int i = 0; i < crates.size(); i++)
                        {
                            crates[i]->displayInfo();
                        }

                        printC("\033[1;34m========== SHIPMENTS ==========\033[0m\n");
                        for (int i = 0; i < shipments.size(); i++)
                        {
                            cout << "ID: " << shipments[i]->getOrderID() << " | " << shipments[i]->getOriginNode() << " -> " << shipments[i]->getDestinationNode() << "\n";
                            cout << "    Status: " << shipments[i]->getStatus() << "\n";
                        }

                        int crateID, shipID;
                        cout << "\nEnter Crate ID to assign: "; cin >> crateID;
                        cout << "Enter Shipment Order ID: "; cin >> shipID;

                        CargoCrate* targetCrate = nullptr;
                        for (int i = 0; i < crates.size(); i++)
                        {
                            if (crates[i]->getCrateID() == crateID)
                            {
                                targetCrate = crates[i];
                                break;
                            }
                        }

                        if (targetCrate == nullptr)
                        {
                           cout << "\033[1;31mCrate ID not found.\033[0m\n";
                            cin.ignore(1000, '\n');
                            cin.get();
                            break;
                        }

                        bool assigned = false;
                        for (int i = 0; i < shipments.size(); i++)
                        {
                            if (shipments[i]->getOrderID() == shipID)
                            {
                                shipments[i]->addCrate(targetCrate);
                                cout << "\033[1;32mCrate " << crateID << " assigned to shipment " << shipID << "\033[0m\n";
                                assigned = true;
                                break;
                            }
                        }

                        if (!assigned)
                        {
                            cout << "\033[1;31mShipment ID not found.\033[0m\n";
                        }
                        cin.ignore(1000, '\n');
                        cin.get();
                        break;
                    }

                    case 8: // add a brand new vehicle to the registry
                    {
                        try
                        {
                            addVehicle(fleet);
                        }
                        catch (const LogisticsException& e)
                        {
                            cout << "\033[1;31mError: " << e.getMessage() << "\033[0m\n";
                        }
                        cin.ignore(1000, '\n');
                        cin.get();
                        break;
                    }

                    case 9: // save everything to text files using DataManager
                    {
                       printC("\033[1;34m==== SAVE DATA ====\033[0m\n");
                        try
                        {
                            dataManager.saveAll(crates, shipments);
                        }
                        catch (const LogisticsException& e)
                        {
                            cout << "\033[1;31m" << e.getMessage() << "\033[0m\n";
                        }
                        cin.ignore(1000, '\n');
                        cin.get();
                        break;
                    }

                    case 10: // load everything from text files using DataManager
                    {
                        printC("\033[1;34m==== LOAD DATA ====\033[0m\n");
                        try
                        {
                            dataManager.loadAll(crates, shipments);
                        }
                        catch (const LogisticsException& e)
                        {
                            cout << "\033[1;31m" << e.getMessage() << "\033[0m\n";
                        }
                        cin.ignore(1000, '\n');
                        cin.get();
                        break;
                    }

                    case 11:
                    {
                        printC("Exiting....\n");
                        break;
                    }

                    default:
                    {
                        break;
                    }
        }

    } while (userChoice != 11);

    // clean up heap memory before program exits
    for (int i = 0; i < crates.size(); i++)
    {
        delete crates[i];
    }
    for (int i = 0; i < shipments.size(); i++)
    {
        delete shipments[i];
    }
    for (int i = 0; i < fleet.size(); i++)
    {
        delete fleet[i];
    }

    return 0;
}
