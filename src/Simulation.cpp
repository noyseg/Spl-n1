#include "Simulation.cpp"
#include <iostream>
#include "Auxiliary.cpp"
#include "Action.h"
using namespace std;
extern Simulation* backup; 

Simulation ::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions()
{
}

void Simulation ::start()
{
    isRunning = true;
    while (isRunning)
    {
        string input;
        cin >> input;
        vector<std::string> vectorInput = Auxiliary::parseArguments(input);
        // close or log 
        if (vectorInput.size() == 1){
            if (vectorInput[0] == "close"){
                close();
            }
            else if (vectorInput[0] == "log"){
                PrintActionsLog().toString();
                // what here???? 
                actionsLog.push_back(restoreSimulation);
            }
            else if (vectorInput[0] == "backup"){
                // remeber to remove last backup 
                BackupSimulation *lastBackup = new BackupSimulation();
                lastBackup.act(*this);
                actionsLog.push_back(lastBackup);
            }
            else if (vectorInput[0] == "restore"){
                // remeber to remove last resotre if needed  
                RestoreSimulation restoreSimulation = new restoreSimulation();
                restoreSimulation.act(*this);
                if (restoreSimulation.getStatus() == ActionStatus::ERROR)
                {
                    cout << restoreSimulation.toString() << endl; 
                }
                actionsLog.push_back(restoreSimulation);
            }
            else{
                cout << "INPUT ERROR" << endl
            }
        }
        if (vectorInput.size() == 2){
            // Print plan status  
            // check is valid 
            if (vectorInput[0] == "PlanStatus" && isValidPlan(std::stoi(vectorInput[1]))==true)
            {
                plans[std::stoi(vectorInput[1])].printStatus();
            }
            else if (vectorInput[0] == "step"){
                SimulateStep simulateStep(std::stoi(vectorInput[1]));
                simulateStep.act(*this);
            }
            else{
                cout << "INPUT ERROR" << endl
            }
        if (vectorInput.size() == 3){

        }
            

    }
}

void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    if (isSettlementExists(settlement.getName()) == false)
    {
        planCounter++; 
        vector<FacilityType> &rFacilitiesOptions = facilitiesOptions;
        Plan *newPlan = new Plan(planCounter, settlement, selectionPolicy, rFacilitiesOptions);
    }
}

void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

bool Simulation::addSettlement(Settlement *settlement)
{
    string settlementName = settlement->getName();
    if (isSettlementExists(settlementName) == false)
    {
        settlements.push_back(settlement);
        return true;
    }
    return false;
}

void Simulation::step()
{
    for (Plan p : plans)
    {
        p.step();
    }
}

void Simulation::close()
{
    for (Plan p : plans)
    {
        p.printStatus();
    }
    isRunning = false;
    for (BaseAction *ba : actionsLog)
    {
        delete ba;
    }
    for (Settlement *stl : settlements)
    {
        delete stl;
    }
}

void Simulation ::open()
{
    isRunning = true;
}