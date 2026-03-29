#pragma once
#include <string>
using namespace std;

class CargoCrate; //forward declaration needed for operator+ parameters

class TransportAsset
{
protected:
    const int assetID;          // an ID for the vehicle
    double maxPayLoadWeight;   // max weight capacity of that vehicle
    double currentLoad;         // the current load
    string assetName;           // the name of the vehicle
    string operationalStatus;   // maintainance, or working , or retired, etc.
    string homeDepot;           // where the home/base of the vehicle is

public:
    TransportAsset(int id, double maxload, double load, string name, string status, string depot);
    //pure virtual function
    virtual double calculateTransitTime(double distance) const = 0;
    //more methods later
    int getAssetID() const;




};


class GroundTransport: virtual public TransportAsset
{
protected:
    string licensePlate;        // license plate for customs audit later prob
    double speedKmph;           // average speed, probably used for transit time later
    string roadClearanceLevel;  // local, highway, offroad, water etc
    double fuelLevel;           // from 1-100 pc


public:
    GroundTransport(int id, double maxload, double load, string name, string status, string depot, string plate, double speed, string level, double fuel);

    double calculateTransitTime(double distance) const override;

    void operator+(const CargoCrate& crate);



};

class AirTransport: virtual public TransportAsset
{
protected:
    string tailNumber;          // aircraft registration
    double cruiseAltitudeM;     // in meters
    double airSpeedKmph;        // in kmph
    int maxFlightRangeKm;       // max range in km
    bool requiresRunway;        // false for drones/VTOL

public:
    AirTransport(int id, double maxload, double load, string name, string status, string depot, string num, double alt, double speed, int range, bool runway);

    double calculateTransitTime(double distance) const override;

    void operator+(const CargoCrate& crate);

};

class HeavyLiftDrone: public GroundTransport, public AirTransport
{
private:
    string droneModel;
    double batteryLevel;       // from 1-100 pc
    bool isAutonomous;         // always true, but still good to flag anyway
    int maxHoverTimeMinutes;   // minutes time before it dies
    double urbanSpeedKmph;     // slower in city corridors
    double aerialSpeedKmph;    // open-air speed


public:
    HeavyLiftDrone(int id, double maxload, double load, string name, string status, string depot, string plate, double Gspeed, string level, double fuel, string num, double alt, double Aspeed, int range, bool runway, string Dmodel, double battery, bool auton, int hover, double HDGspeed, double HDAspeed);

    double calculateTransitTime(double distance) const override;

    void operator+(const CargoCrate& crate);




};

class WaterTransport: virtual public TransportAsset
{
private:
    string vesselRegistrationNumber;
    double speedKnots;              // water speed in knots, cuz thats what ships use
    string vesselType;              // cargo ship or ferry or barge, etc
    double draughtMeters;           // how deep it sits in water
    int maxCrewCapacity;
    string portOfRegistry;          // home port
    bool isOceanGoing;              // false = river/canal only


public:
    WaterTransport(int id, double maxload, double load, string name, string status, string depot, string Rnum, double knots, string type, double draught, int maxcrew, string portreg, bool ocean);

    void operator+(const CargoCrate& crate);

    double calculateTransitTime(double distance) const override;
};
