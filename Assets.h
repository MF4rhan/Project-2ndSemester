#pragma once
#include <string>
#include <iostream>
#include "Exceptions.h"
using namespace std;

class CargoCrate; // forward declaration needed for operator+ parameters

class TransportAsset
{
protected:
    const int assetID;
    double maxPayLoadWeight;
    double currentLoad;
    string assetName;
    string operationalStatus;
    string homeDepot;

public:
    TransportAsset(int id, double maxload, double load, string name, string status, string depot);

    virtual double calculateTransitTime(double distance) const = 0;
    virtual void loadCrate(const CargoCrate& crate) = 0;  // unified load interface
    virtual string getTypeName() const = 0; // "Ground", "Air", "Water", "Drone"

    int getAssetID() const;
    double getCurrentLoad() const;
    double getMaxPayLoad() const;
    string getStatus() const;
    string getName() const;

    void resetLoad();

    virtual ~TransportAsset() {}
};


class GroundTransport : virtual public TransportAsset
{
protected:
    string licensePlate;
    double speedKmph;
    string roadClearanceLevel;
    double fuelLevel;

public:
    GroundTransport(int id, double maxload, double load, string name, string status, string depot, string plate, double speed, string level, double fuel);

    double calculateTransitTime(double distance) const override;
    void operator+(const CargoCrate& crate);
    void loadCrate(const CargoCrate& crate) override;
    string getTypeName() const override;
};


class AirTransport : virtual public TransportAsset
{
protected:
    string tailNumber;
    double cruiseAltitudeM;
    double airSpeedKmph;
    int maxFlightRangeKm;
    bool requiresRunway;

public:
    AirTransport(int id, double maxload, double load, string name, string status, string depot, string num, double alt, double speed, int range, bool runway);

    double calculateTransitTime(double distance) const override;
    void operator+(const CargoCrate& crate);
    void loadCrate(const CargoCrate& crate) override;
    string getTypeName() const override;
};


class HeavyLiftDrone : public GroundTransport, public AirTransport
{
private:
    string droneModel;
    double batteryLevel;
    bool isAutonomous;
    int maxHoverTimeMinutes;
    double urbanSpeedKmph;
    double aerialSpeedKmph;

public:
    HeavyLiftDrone(int id, double maxload, double load, string name, string status, string depot, string plate, double Gspeed, string level, double fuel,
                   string num, double alt, double Aspeed, int range, bool runway, string Dmodel, double battery, bool auton, int hover,
                   double HDGspeed, double HDAspeed);

    double calculateTransitTime(double distance) const override;
    void operator+(const CargoCrate& crate);
    void loadCrate(const CargoCrate& crate) override;
    string getTypeName() const override;
};


class WaterTransport : virtual public TransportAsset
{
private:
    string vesselRegistrationNumber;
    double speedKnots;
    string vesselType;
    double draughtMeters;
    int maxCrewCapacity;
    string portOfRegistry;
    bool isOceanGoing;

public:
    WaterTransport(int id, double maxload, double load, string name, string status, string depot, string Rnum, double knots, string type, double draught,
                   int maxcrew, string portreg, bool ocean);

    double calculateTransitTime(double distance) const override;
    void operator+(const CargoCrate& crate);
    void loadCrate(const CargoCrate& crate) override;
    string getTypeName() const override;
};
