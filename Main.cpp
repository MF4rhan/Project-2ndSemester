#include "Assets.h"
#include "Logistics.h"

void printC(string message, int width = 140) {
    int padding = (width - (int)message.length()) / 2;    // .length returns size_t (unsigned), so convert to int to remove warning
    cout << string(padding, ' ') << message;
}

int ShipmentOrder::orderCount = 0;

int main() {
    CargoCrate* crates[50]; //array of crates
    int crateCount = 0;
    ShipmentOrder* shipments[50]{};
    int shipmentCount = 0;


    CustomsAuditor auditor(1, "Ahmed", "GULF", "", 20);
    GroundTransport Truck1(123, 7000, 0, "Truck", "working", "Karachi", "ABC-123", 80, "Highway", 100); //temporary truck object
    AirTransport CargoPlane1(234, 10000, 0, "Cargo Plane", "working", "Karachi", "AC-12", 12000, 700, 3000, true); //temporary Ship
    HeavyLiftDrone Drone1(90, 3000, 0, "Heavy Lift Drone", "working", "Karachi", "A-12", 60, "Local", 100, "BD-14", 1000, 80, 500, false, "ASJD100FK", 100, true, 180, 30, 60); //temporary Heavy
    WaterTransport Ship1(400, 50000, 0, "Shippy", "working", "Karachi", "ATY-19", 5, "Cargo Ship", 30, 20, "Karachi Port", true); //temporary Ship


    int userChoice = 1;   //avoid unwanted prompt at range check at do while's start

    do {
        //system("clear");    //for linux, use system("clear");
        system("cls");    //for Windows, use system("cls");
        if (userChoice < 1 || userChoice > 7) {
            printC("--- Incorrect input, please try again ---\n");
        }
        printC("==== = SUPPLY CHAING LOGISTICS = ====\n");
        printC("1. Create new shipment\n");
        printC("2. Load cargo onto vehicle\n");
        printC("3. Dispatch shipment\n");
        printC("4. Run customs audit\n");
        printC("5. View fleet status\n");
        printC("6. Complete shipment\n");
        printC("7. Assign crate to shipment\n");
        printC("8. Exit\n");
        cin >> userChoice;

        switch (userChoice) {
        case 1:
        {

            int ch; string origin; string destination; double totalWeight; string priorityLevel; string status; string cargoType; bool international; int assignedAssetID;

            printC("Enter origin location: \n");                                                                  cin >> origin;
            printC("Enter destination: \n");                                                                      cin >> destination;
            printC("Enter total weight: \n");                                                                     cin >> totalWeight;
            printC("Select priority level: \n1. STANDARD\n2. EXPRESS\n 3. CRITICAL\n");                           cin >> ch;
            printC("Select status: \n1. ON\n2. OFF");                                                             cin >> status;
            printC("Enter cargo type: ");                                                                         cin >> cargoType;
            printC("Is it international?: ");                                                                     cin >> international;
            printC("Enter assigned asset ID: ");                                                                  cin >> assignedAssetID;


            switch (ch) {
            case 1: priorityLevel = "STANDARD"; break;
            case 2: priorityLevel = "EXPRESS"; break;
            case 3: priorityLevel = "CRITICAL"; break;
            default: priorityLevel = "STANDARD"; break;
            }
            printC("Select status: \n1. PENDING\n2. IN_TRANSIT\n 3. DELIVERED\n");                                cin >> ch;
            switch (ch) {
            case 1: status = "PENDING"; break;
            case 2: status = "IN_TRANSIT"; break;
            case 3: status = "DELIVERED"; break;
            default: status = "PENDING"; break;
            }
            printC("Select cargo type: \n1. GENERAL\n2. PERISHABLE\n3. HAZARDOUS\n");                             cin >> ch;
            switch (ch) {
            case 1: cargoType = "GENERAL"; break;
            case 2: cargoType = "PERISHABLE"; break;
            case 3: cargoType = "HAZARDOUS"; break;
            default: cargoType = "GENERAL"; break;
            }
            printC("Is shipment international? (1 = Yes, 0 = No): \n");                                           cin >> ch;
            switch (ch) {
            case 1: international = true; break;
            case 0: international = false; break;
            default: international = true; break;
            }
            printC("Assign to which vehicle?: \n1. Truck\n2. Cargo Plane\n 3. Ship\n 4. Heavy Lift Drone\n");     cin >> ch;
            switch (ch) {
            case 1: assignedAssetID = Truck1.getAssetID(); break;
            case 2: assignedAssetID = CargoPlane1.getAssetID(); break;
            case 3: assignedAssetID = Ship1.getAssetID(); break;
            case 4: assignedAssetID = Drone1.getAssetID(); break;
            default: assignedAssetID = -1; break;
            }
            shipments[shipmentCount++] = new ShipmentOrder(origin, destination, totalWeight, priorityLevel, status, cargoType, international, assignedAssetID);

            break;
        }

        case 2:
        {
            printC("Select Vehicle: \n\n");
            printC("1. Truck\n");
            printC("2. Cargo Plane\n");
            printC("3, Ship\n");
            printC("4. Heavy Lift Drone\n");
            int choice;
            while (1)
            {
                cin >> choice;
                if (choice == 1 || choice == 2 || choice == 3 || choice == 4)
                    break;
                else
                    printC("Invalid choice, Please try again.\n");
            }
            cout << endl;
            printC("Enter Crate ID: \n");
            int tempID;
            cin >> tempID;

            printC("Enter Content Description: \n");
            string tempDesc;
            cin >> tempDesc;

            printC("Enter Weight (Kg): \n");
            double tempWeight;
            cin >> tempWeight;

            printC("Fragility (FRAGILE / STANDARD): \n");
            string tempFrag;
            cin >> tempFrag;

            printC("Hazmat Code (NONE / FLAMMABLE / BIOLOGICAL / RADIOACTIVE): \n");
            string tempHam;
            cin >> tempHam;

            printC("Enter Origin Country: \n");
            string tempcount;
            cin >> tempcount;

            crates[crateCount] = new CargoCrate(tempID, tempDesc, tempWeight, tempFrag, tempHam, tempcount);
            if (choice == 1)
                Truck1 + *crates[crateCount]; //check later
            else if (choice == 2)
                CargoPlane1 + *crates[crateCount]; //check later
            else if (choice == 3)
                Ship1 + *crates[crateCount]; //check later
            else if (choice == 4)
                Drone1 + *crates[crateCount]; //check later
            crateCount++;
            //more later
            break;
        }
        case 3: {//update 3
            for (int i = 0; i < shipmentCount; i++)
            {
                printC("Order ID: "); cout << shipments[i]->getOrderID() << "\t";
                printC("Priority Level: "); cout << shipments[i]->getPriority() << endl;
            }
            cout << endl;
            printC("Enter Shipment Order ID to dispatch:");
            int tempID;
            cin >> tempID;

            printC("Select Vehicle to assign: \n");
            printC("1. Truck");
            printC("2. Cargo Plane");
            printC("3, Ship");
            printC("4. Heavy Lift Drone");
            int choice;
            while (1)
            {
                cin >> choice;
                if (choice == 1 || choice == 2 || choice == 3 || choice == 4)
                    break;
                else
                    printC("Invalid choice, Please try again.");
            }

            printC("Enter distance to destination (Km): ");
            int tempDis;
            cin >> tempDis;

            for (int i = 0; i < shipmentCount; i++)
            {
                if (shipments[i]->getOrderID() == tempID)
                {
                    shipments[i]->setStatus("IN_TRANSIT");

                }
            }

            if (choice == 1)
                Truck1; //check later //I don't understand what or how am i supposed to calculate transit time with just orderID
            else if (choice == 2)
                CargoPlane1 + *crates[crateCount]; //check later
            else if (choice == 3)
                Ship1 + *crates[crateCount]; //check later
            else if (choice == 4)
                Drone1 + *crates[crateCount]; //check later
            crateCount++;

            break;
        }

        case 4: {

            int ID; bool flag = false;

            while (flag == false) {
                printC("Enter Shipment Order ID to audit: ");                               cin >> ID;
                for (int i = 0; i < shipmentCount; i++) {
                    if (shipments[i]->getOrderID() == ID) {
                        flag = true;
                        if (!shipments[i]->getIsInternational()) {
                            printC("Domestic shipment - no audit required\n");
                        }
                        else {
                            auditor.auditShipment(*shipments[i]);
                        }
                    }
                }

                if (!flag) {
                    printC("ID not found, try again\n");
                }
            }

              break;
        }

            case 5:

                break;

            case 6:

                break;

            case 7:

                break;

            case 8:
                printC("Exiting....");
                break;

            default:
                break;
            }

    } while (userChoice != 7);


    return 0;
}
