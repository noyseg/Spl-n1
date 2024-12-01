#include "Simulation.h"

Simulation ::Simulation(const string &configFilePath) : isRunning(false),planCounter(0),actionsLog(), plans(),settlements(),facilitiesOptions()
{
   
}

void Simulation ::start(){
    isRunning = true;
}

void Simulation:: addPlan(const)