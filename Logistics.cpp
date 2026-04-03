#include "Logistics.h"
#include <iostream>
using namespace std;


//static member defintion
int ShipmentOrder::globalActiveShipments = 0;

//Shipment order
ShipmentOrder::ShipmentOrder(string oN, string dest, double Tw, string pL, string stat, string type, bool isInt, int AID)
:orderID(orderCount++), originNode(oN), destinationNode(dest), totalWeightKg(Tw), priorityLevel(pL), status(stat), cargoType(type), isInternational(isInt), assignedAssetID(AID)//update 1
{
    globalActiveShipments++;

    for (int i = 0; i < 20; i++) {
        assignedCrates[i] = nullptr;
    }
}

void ShipmentOrder::addCrate(CargoCrate* crate)
{
    if (crateCount < 20) {
        assignedCrates[crateCount++] = crate;
        totalWeightKg += crate->getWeight();
    }
}

void ShipmentOrder:: renderGlobalDashboard()
{
    //logic  later
}

ShipmentOrder::~ShipmentOrder()
{
    globalActiveShipments--;
}


int ShipmentOrder::getOrderID() const {return orderID;} //update 1
string ShipmentOrder::getOriginNode() const {return originNode;};
string ShipmentOrder::getDestinationNode() const {return destinationNode;};
double ShipmentOrder::getTotalWeight() const {return totalWeightKg;};
string ShipmentOrder::getPriority() const {return priorityLevel;} //update 1
void ShipmentOrder::setStatus(string stat)  {status=stat;} //update 2
bool ShipmentOrder::getIsInternational() const {return isInternational;}
int ShipmentOrder::getCrateCount() const {return crateCount;};

//Cargo Crate
CargoCrate::CargoCrate(int ID, string cD, double W, string fF, string ham, string country)
: crateID(ID), contentsDescription(cD), weightKg(W), fragileFlag(fF), hazmatCode(ham), originCountry(country) {}

double CargoCrate::getWeight() const        {return weightKg;}
string CargoCrate::getHazmat() const        {return hazmatCode;}
string CargoCrate::getFragileFlag() const   {return fragileFlag;}

CargoCrate CargoCrate::operator+(const CargoCrate& other) const
{
    //logic later
    return *this; //placeholder for now
}

//Customs Auditor
CustomsAuditor::CustomsAuditor(int ID, string name, string jurs, string ban, double trump)
: auditorID(ID), auditorName(name), jurisdiction(jurs), tariffRatePercent(trump)
{
    // in-class array init not allowed for plain arrays outside C++11 aggregate init
    // so initialize here instead
    bannedHamzatCodes[0] = "UN0190";
    bannedHamzatCodes[1] = "UN1789";
    bannedHamzatCodes[2] = "UN1203";
    bannedHamzatCodes[3] = "UN1845";
    bannedHamzatCodes[4] = "UN3480";
    // [5]-[9] left empty for future additions
}

void CustomsAuditor::auditShipment(const ShipmentOrder& order) {

    cout << ("=== CUSTOMS AUDIT ===\n");
    cout << "Origin: " << order.originNode << endl;
    cout << "Destination: " << order.destinationNode << endl;
    cout << "Total weight: " << order.totalWeightKg << "kg" << endl;
    cout << "Crates in shipment: " << order.crateCount << endl;
    cout << "------------------------------------" << endl;

    for (int i = 0; i < order.crateCount; i++) {
        CargoCrate* c = order.assignedCrates[i];

        cout << "Crate #" << i + 1 << " | " << c->getWeight() << "kg" << " | Hazmat: " << c->getHazmat() << endl;

        bool banned = false;

        for (int j = 0; j < 10; j++) {
            if (bannedHamzatCodes[j] == c->getHazmat()) {
                banned = true;
                break;
            }
        }

        if (banned) {
            cout << "\033[1;31m[FLAGGED] Banned hamzat detected!\033[0m" << endl;
        }
        else {
            cout << "\033[1; 32m[CLEARED] Crate passed inspection.\033[0m" << endl;
        }
        
        if (c->getFragileFlag() == "FRAGILE") {
            cout << "\033[1;33m[WARNING] Fragile crate!\033[0m" << endl;
        }

    }

    double tariff = order.totalWeightKg * tariffRatePercent / 100;
    cout << "Total tariff: $" << tariff << endl;
}
