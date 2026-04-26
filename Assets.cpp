#include "Assets.h"
#include "Logistics.h"
using namespace std;

//TransportAssets stuff
TransportAsset::TransportAsset(int id, double maxload, double load, string name, string status, string depot)
:assetID(id), maxPayLoadWeight(maxload), currentLoad(load), assetName(name), operationalStatus(status), homeDepot(depot)   {}

int TransportAsset::getAssetID() const  {return assetID;}
double TransportAsset::getCurrentLoad() const {return currentLoad;}
double TransportAsset::getMaxPayLoad() const {return maxPayLoadWeight;}
string TransportAsset::getStatus() const {return operationalStatus;}
string TransportAsset::getName() const {return assetName;}

void TransportAsset::resetLoad() {currentLoad = 0;}

//Ground Transport stuff
GroundTransport::GroundTransport(int id, double maxload, double load, string name, string status, string depot, string plate, double speed, string level, double fuel)
:TransportAsset(id, maxload, load, name, status, depot), licensePlate(plate), speedKmph(speed), roadClearanceLevel(level), fuelLevel(fuel)  {}

double GroundTransport::calculateTransitTime(double distance) const
{
    // time (hrs) = distance / speed. also a guard against zero speed
    if (speedKmph <= 0)
    {
        return -1;
    }
    return distance / speedKmph;
}

void GroundTransport::operator+(const CargoCrate& crate)
{
    if (currentLoad + crate.getWeight() > maxPayLoadWeight)
    {
        throw PayloadExceededException(assetName, currentLoad + crate.getWeight(), maxPayLoadWeight);
    }
    currentLoad += crate.getWeight();
}

void GroundTransport::loadCrate(const CargoCrate& crate) {(*this) + crate;}
string GroundTransport::getTypeName() const {return "Ground";}



//Air Transport stuff

AirTransport::AirTransport(int id, double maxload, double load, string name, string status, string depot, string num, double alt, double speed, int range, bool runway)
:TransportAsset(id, maxload, load, name, status, depot), tailNumber(num), cruiseAltitudeM(alt), airSpeedKmph(speed), maxFlightRangeKm(range), requiresRunway(runway)    {}

double AirTransport::calculateTransitTime(double distance) const
{
    if (airSpeedKmph <= 0)
    {
        return -1;
    }
    return distance / airSpeedKmph;
}

void AirTransport::operator+(const CargoCrate& crate)
{
    if (crate.getHazmat() != "NONE")
    {
        throw CargoRestrictionException(assetName, "hazmat cargo '" + crate.getHazmat() + "' is not permitted on Air Transport.");
    }
    if (currentLoad + crate.getWeight() > maxPayLoadWeight)
    {
        throw PayloadExceededException(assetName, currentLoad + crate.getWeight(), maxPayLoadWeight);
    }
    currentLoad += crate.getWeight();
}

void AirTransport::loadCrate(const CargoCrate& crate) {(*this) + crate;}
string AirTransport::getTypeName() const {return "Air";}

//HeavyLiftDrone stuff
HeavyLiftDrone::HeavyLiftDrone(int id, double maxload, double load, string name, string status, string depot, string plate, double Gspeed, string level, double fuel, string num, double alt, double Aspeed, int range, bool runway, string Dmodel, double battery, bool auton, int hover, double HDGspeed, double HDAspeed)
:TransportAsset(id, maxload, load, name, status, depot), GroundTransport(id, maxload, load, name, status, depot, plate, Gspeed, level, fuel), AirTransport(id, maxload, load, name, status, depot, num, alt, Aspeed, range, runway), droneModel(Dmodel), batteryLevel(battery), isAutonomous(auton), maxHoverTimeMinutes(hover), urbanSpeedKmph(HDGspeed), aerialSpeedKmph(HDAspeed)  {}

double HeavyLiftDrone::calculateTransitTime(double distance) const
{
    // using aerial speed for open air legs, falling back to urban speed if aerial is 0
    double speed = (aerialSpeedKmph > 0) ? aerialSpeedKmph : urbanSpeedKmph;
    if (speed <= 0)
    {
        return -1;
    }
    return distance / speed;
}

void HeavyLiftDrone::operator+(const CargoCrate& crate)
{
    if (crate.getHazmat() != "NONE")
    {
        throw CargoRestrictionException(assetName, "hazmat cargo '" + crate.getHazmat() + "' is not permitted on Drone Transport.");
    }
    if (crate.getWeight() > 50.0)
    {
        throw CargoRestrictionException(assetName, "single crate weight " + to_string(crate.getWeight()) + " kg exceeds drone's 50 kg per-crate limit.");
    }
    if (currentLoad + crate.getWeight() > maxPayLoadWeight)
    {
        throw PayloadExceededException(assetName, currentLoad + crate.getWeight(), maxPayLoadWeight);
    }
    currentLoad += crate.getWeight();
}

void HeavyLiftDrone::loadCrate(const CargoCrate& crate) {(*this) + crate;}
string HeavyLiftDrone::getTypeName() const {return "Drone";}

//Water Transport stuff
WaterTransport::WaterTransport(int id, double maxload, double load, string name, string status, string depot, string Rnum, double knots, string type, double draught, int maxcrew, string portreg, bool ocean)
:TransportAsset(id, maxload, load, name, status, depot), vesselRegistrationNumber(Rnum), speedKnots(knots), vesselType(type), draughtMeters(draught), maxCrewCapacity(maxcrew), portOfRegistry(portreg), isOceanGoing(ocean)    {}

double WaterTransport::calculateTransitTime(double distance) const
{
    // convert knots to km/h, as 1 knot = 1.852 km/h
    double speedKmph = speedKnots * 1.852;
    if (speedKmph <= 0)
    {
        return -1;
    }
    return distance / speedKmph;
}

void WaterTransport::operator+(const CargoCrate& crate)
{
    if (crate.getFragileFlag() == "FRAGILE")
    {
        throw CargoRestrictionException(assetName, "fragile cargo is not permitted on water transport.");
    }
    if (currentLoad + crate.getWeight() > maxPayLoadWeight)
    {
        throw PayloadExceededException(assetName, currentLoad + crate.getWeight(), maxPayLoadWeight);
    }
    currentLoad += crate.getWeight();
}

void WaterTransport::loadCrate(const CargoCrate& crate) {(*this) + crate;}
string WaterTransport::getTypeName() const {return "Water";}
