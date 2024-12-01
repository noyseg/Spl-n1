#include "Simulation.h"

Simulation ::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions()
{
}

void Simulation ::start()
{
    isRunning = true;
}

bool Simulation::addFacility(FacilityType facility)
{
    facilitiesOptions.push_back(facility);
}

bool Simulation::isSettlementExists(const string &settlementName)
{
    bool isExist = false;
    for (Settlement *stl : settlements)
    {
        if (stl->getName() == settlementName)
        {
            return true;
        }
    }
    return isExist;
}

Settlement &Simulation ::getSettlement(const string &settlementName)
{
    for (Settlement *stl : settlements)
    {
        if (stl->getName() == settlementName)
        {
            return *stl; // Dereference pointer and return the object reference
        }
    }
    // Return error ?
}

Plan &Simulation ::getPlan(const int planId)
{
    // Assuming you can't delete a plan and plan counter starting from 1
    if (planId - 1 >= 0 && planId - 1 < plans.size())
    {
        return plans.at(planId - 1);
    }
    // Return error ?
}

bool Simulation::isValidPlan(const string &id)
{
    int planId = std::stoi(id);
    if (planId - 1 >= 0 && planId - 1 < plans.size())
    {
        return true;
    }
    return false;
}
