#include "Logistics.h"
#include <iostream>
using namespace std;

//static member defintion
int ShipmentOrder::globalActiveShipments = 0;

//Shipment order
ShipmentOrder::ShipmentOrder(string oN, string dest, double Tw, string pL, string stat, string type, bool isInt, int AID)
:orderID(OrderCounter), originNode(oN), destinationNode(dest), totalWeightKg(Tw), priorityLevel(pL), status(stat), cargoType(type), isInternational(isInt), assignedAssetID(AID), OrderCounter(0) //update 1
{
    globalActiveShipments++;
    OrderCounter++; //update 1
}

void ShipmentOrder:: renderGlobalDashboard()
{
    //logic  later
}

ShipmentOrder::~ShipmentOrder()
{
    globalActiveShipments--;
}

int ShipmentOrder::getOrderID() {return orderID;} //update 1
string ShipmentOrder::getPriority() {return priorityLevel;} //update 1
void ShipmentOrder::setStatus(string stat)  {status=stat;} //update 2

//Cargo Crate
CargoCrate::CargoCrate(int ID, string cD, double W, string fF, string ham, string country)
: crateID(ID), contentsDescription(cD), weightKg(W), fragileFlag(fF), hamzatCode(ham), originCountry(country)  {}

double CargoCrate::getWeight() const        {return weightKg;}
string CargoCrate::getHamzat() const        {return hamzatCode;}
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
