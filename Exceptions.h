#pragma once
#include <string>
using namespace std;

// custom base class for all exceptions
class LogisticsException
{
protected:
    string message;
public:
    LogisticsException(string msg)
    {
        message = msg;
    }

    string getMessage() const
    {
        return message;
    }
};

// thrown when a vehicle goes over its max weight capacity
class PayloadExceededException : public LogisticsException
{
public:
    PayloadExceededException(string vName, double tryingToAdd, double maxLoad)
    : LogisticsException("Error: " + vName + " cannot load " + to_string(tryingToAdd) + "kg. Max is " + to_string(maxLoad)) {}
};

// thrown if banned cargo is put on the wrong transport type
class CargoRestrictionException : public LogisticsException
{
public:
    CargoRestrictionException(string vName, string reason)
    : LogisticsException("Restriction on " + vName + ": " + reason) {}
};

// basic file opening/saving errors
class FileIOException : public LogisticsException
{
public:
    FileIOException(string fName, string errorMsg)
    : LogisticsException("File error with " + fName + " -> " + errorMsg) {}
};

// if an id doesn't match anything in the registry
class IDNotFoundException : public LogisticsException
{
public:
    IDNotFoundException(string type, int id)
    : LogisticsException(type + " ID " + to_string(id) + " not found.") {}
};
