#pragma once
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class CargoCrate;

class ShipmentOrder
{
private:
    const int orderID;
    string originNode;
    string destinationNode;
    double totalWeightKg;
    string priorityLevel;       // "STANDARD", "EXPRESS", "CRITICAL"
    string status;              // "PENDING", "IN_TRANSIT, "DELIVERED"
    string cargoType;           // "GENERAL", "PERISHABLE", "HAZARDOUS"
    bool isInternational;       // triggers CustomsAuditor
    int assignedAssetID;     // links to a TransportAsset
    static int orderCount; //update 1
    static int globalActiveShipments;

    CargoCrate* assignedCrates[20];
    int crateCount = 0;

public:
    ShipmentOrder(string oN, string dest, double Tw, string pL, string stat, string type, bool isInt, int AID);
    static void renderGlobalDashboard(); //definition in cpp

    ~ShipmentOrder();

    int getOrderID() const; //update 1
    string getOriginNode() const;
    string getDestinationNode() const;
    double getTotalWeight() const;
    int getCrateCount() const;
    string getPriority() const; //update 1
    void setStatus(string stat); //update 2
    bool getIsInternational() const;
    
    void addCrate(CargoCrate* crate);

    friend class CustomsAuditor;

};

class CargoCrate
{
private:
    int crateID;
    string contentsDescription;
    double weightKg;
    string fragileFlag;             // "FRAGILE", "STANDARD" - for CustomsAuditor
    string hazmatCode;              // "NONE", "FLAMMABLE", "BIOLOGICAL", "RADIOACTIVE"  - for CustomsAuditor
    string originCountry;           // - for CustomsAuditor


public:
    CargoCrate(int ID, string cD, double W, string fF, string ham, string country);
    double getWeight() const;
    string getHazmat() const;
    string getFragileFlag() const;
    CargoCrate operator+(const CargoCrate& other) const;
    //Not sure if this is right, check its syntax later
    //definition in cpp

};

class CustomsAuditor
{
private:
    int auditorID;
    string auditorName;
    string jurisdiction;
    string bannedHamzatCodes[10];   // For CargoCrate, or others
    double tariffRatePercent;


public:
    CustomsAuditor(int ID, string name, string jurs, string ban, double trump);

    void auditShipment(const ShipmentOrder& order);

};
