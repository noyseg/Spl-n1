#include "Settlement.h"

// class Settlement{
// constructor
Settlement::Settlement(const string &name, SettlementType type)
    : name(name), type(type) {}

// return the name of the settlement
const string &Settlement::getName() const
{
    return name;
}

// return the type of the settlement
SettlementType Settlement::getType() const
{
    return type;
}

// Return SettlementType of given int
SettlementType Settlement::stringToSettlementType(const string& type){
    if (type == "0")
    {
        return SettlementType::VILLAGE;
    }
    else if (type == "1")
    {
        return SettlementType::CITY;
    }
    return SettlementType::METROPOLIS;
}
// return the settlement propeties
const string Settlement::toString() const
{
    string str = name;
    if (type == SettlementType::VILLAGE)
    {
        str += " 0";
    }
    else if (type == SettlementType::CITY)
    {
        str += " 1";  
    }
    else
    {
        str += " 2";
    }
    return str;
}
