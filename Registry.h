#pragma once
#include <string>
#include "Exceptions.h"
using namespace std;

// generic array to hold pointers of CargoCrate*, ShipmentOrder*, TransportAsset* so we dont have to rewrite this logic
template <typename T, int MAX_CAPACITY = 50>
class Registry
{
private:
    T arr[MAX_CAPACITY];
    int currSize;

public:
    Registry()
    {
        currSize = 0;
        for (int i = 0; i < MAX_CAPACITY; i++)
        {
            arr[i] = nullptr;
        }
    }

    // add new item to array
    void add(T item)
    {
        if (currSize >= MAX_CAPACITY)
        {
            throw LogisticsException("Registry is full! Max capacity: " + to_string(MAX_CAPACITY));
        }
        arr[currSize] = item;
        currSize++;
    }

    // safely get an item with bounds checking
    T get(int index) const
    {
        if (index < 0 || index >= currSize)
        {
            throw LogisticsException("Index " + to_string(index) + " is invalid. Current size: " + to_string(currSize));
        }
        return arr[index];
    }

    int size() const
    {
        return currSize;
    }

    bool empty() const
    {
        return currSize == 0;
    }

    T operator[](int index) const
    {
        return arr[index];
    }
};
