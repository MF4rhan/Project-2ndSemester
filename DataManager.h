#pragma once
#include "Logistics.h"
#include "Assets.h"
#include "Exceptions.h"
#include "Registry.h"
#include <fstream>
#include <sstream>
using namespace std;

// this file handles saving and loading to text files using comma separated values
class DataManager
{
private:
    string crateFile = "crates.txt";
    string shipFile = "shipments.txt";

    // helper function to cut up a string based on commas
    static int chopLine(string line, char delim, string pieces[], int maxPieces)
    {
        int count = 0;
        stringstream ss(line);
        string tempToken;

        while (getline(ss, tempToken, delim) && count < maxPieces)
        {
            pieces[count] = tempToken;
            count++;
        }
        return count;
    }

public:
    // --- SAVING DATA ---

    void saveCrates(const Registry<CargoCrate*>& crateList) const
    {
        ofstream out(crateFile);
        if (!out.is_open())
        {
            throw FileIOException(crateFile, "Failed to open file for writing.");
        }

        for (int i = 0; i < crateList.size(); i++)
        {
            CargoCrate* c = crateList[i];
            // format: id,weight,desc,fragile,hazmat,country
            out << c->getCrateID() << "," << c->getWeight() << "," << "None" << ","
            << c->getFragileFlag() << "," << c->getHazmat() << "," << "None" << "\n";
        }

        out.close();
        cout << "Saved " << crateList.size() << " crates to " << crateFile << endl;
    }

    void saveShipments(const Registry<ShipmentOrder*>& shipList) const
    {
        ofstream out(shipFile);
        if (!out.is_open()) {
            throw FileIOException(shipFile, "Failed to open file for writing.");
        }

        for (int i = 0; i < shipList.size(); i++)
        {
            ShipmentOrder* s = shipList[i];
            // format: id,origin,dest,weight,priority,status,type,intl,assetID,crateCount
            out << s->getOrderID() << "," << s->getOriginNode() << "," << s->getDestinationNode() << ","
            << s->getTotalWeight() << "," << s->getPriority() << "," << s->getStatus() << ","
            << s->getCargoType() << "," << s->getIsInternational() << "," << s->getAssignedAssetID() << ","
            << s->getCrateCount() << "\n";
        }

        out.close();
        cout << "Saved " << shipList.size() << " shipments to " << shipFile << endl;
    }

    void saveAll(const Registry<CargoCrate*>& crates, const Registry<ShipmentOrder*>& shipments) const
    {
        saveCrates(crates);
        saveShipments(shipments);
        cout << "All data saved successfully.\n";
    }

    // --- LOADING DATA ---

    void loadCrates(Registry<CargoCrate*>& crateList) const
    {
        ifstream in(crateFile);
        if (!in.is_open())
        {
            throw FileIOException(crateFile, "File not found. Save data first.");
        }

        string currLine;
        int loaded = 0;

        while (getline(in, currLine))
        {
            if (currLine == "")
                continue;

            string parts[6];
            int pCount = chopLine(currLine, ',', parts, 6);
            if (pCount < 6)
                continue; // skip bad lines

            int id = stoi(parts[0]);
            double weight = stod(parts[1]);
            string desc = parts[2];
            string fragile = parts[3];
            string hazmat = parts[4];
            string country = parts[5];

            crateList.add(new CargoCrate(id, weight, desc, fragile, hazmat, country));
            loaded++;
        }

        in.close();
        cout << "Loaded " << loaded << " crates.\n";
    }

    void loadShipments(Registry<ShipmentOrder*>& shipList) const
    {
        ifstream in(shipFile);
        if (!in.is_open())
        {
            throw FileIOException(shipFile, "File not found. Save data first.");
        }

        string currLine;
        int loaded = 0;

        while (getline(in, currLine))
        {
            if (currLine == "") continue;

            string parts[10];
            int pCount = chopLine(currLine, ',', parts, 10);
            if (pCount < 10) continue;

            // parts[0] is orderID but it auto increments in the constructor so we skip it
            string origin = parts[1];
            string dest = parts[2];
            double weight = stod(parts[3]);
            string priority = parts[4];
            string status = parts[5];
            string type = parts[6];
            bool isIntl = (parts[7] == "1");
            int assetID = stoi(parts[8]);

            ShipmentOrder* newShip = new ShipmentOrder(origin, dest, weight, priority, status, type, isIntl, assetID);
            newShip->setStatus(status); // force correct status
            shipList.add(newShip);
            loaded++;
        }

        in.close();
        cout << "Loaded " << loaded << " shipments.\n";
    }

    void loadAll(Registry<CargoCrate*>& crates, Registry<ShipmentOrder*>& shipments) const
    {
        loadCrates(crates);
        loadShipments(shipments);
        cout << "All data loaded successfully.\n";
    }
};
