#include "Simulation.h"
#include "Settlement.cpp"
#include <iostream>
#include "Auxiliary.cpp"
#include "Action.cpp"
#include <fstream>
#include <string>
using namespace std;

Simulation ::Simulation(const string &configFilePath) : isRunning(false), planCounter(0), actionsLog(), plans(), settlements(), facilitiesOptions()
{
    std::ifstream File(configFilePath);
    string line;
    while (getline(File, line))
    {
        vector<std::string> read = Auxiliary::parseArguments(line);
        if (read[0] == "settlement")
        {
            Settlement *stl = new Settlement(read[1], Settlement::stringToSettlementType(read[2]));
            addSettlement(stl);
        }
        else if (read[0] == "facility")
        {
            Facility *stl = new Facility(read[1], , std::stoi(read[3]), std::stoi(read[4]), std::stoi(read[5]), std::stoi(read[6]));
            addSettlement(stl);
        }
    }
}

BaseAction *Simulation::navigateAction(vector<std::string> vectorInput)
{
    if (vectorInput.size() == 1)
    {
        if (vectorInput[0] == "log")
        {
            return new PrintActionsLog();
        }
        else if (vectorInput[0] == "backup")
        {
            return new BackupSimulation();
        }
        else if (vectorInput[0] == "restore")
        {
            // remeber to remove last resotre if needed
            return new RestoreSimulation();
        }
    }
    if (vectorInput.size() == 2)
    {
        // Print plan status
        // check is valid
        if (vectorInput[0] == "PlanStatus" && isValidPlan(std::stoi(vectorInput[1])) == true)
        {
            return new PrintPlanStatus(std::stoi(vectorInput[1]));
        }
        else if (vectorInput[0] == "step")
        {
            return new SimulateStep(std::stoi(vectorInput[1]));
        }
    }
    if (vectorInput.size() == 3)
    {
        if (vectorInput[0] == "settlement")
        {
            SettlementType stlType = {
                std::stoi(vectorInput[2]) == 0 ? SettlementType::VILLAGE : std::stoi(vectorInput[2]) == 1 ? SettlementType::CITY
                                                                                                          : SettlementType::METROPOLIS};
            return new AddSettlement(vectorInput[1], stlType);
        }
        else if (vectorInput[0] == "plan")
        {
            return new AddPlan(vectorInput[1], vectorInput[2]);
        }
        else if (vectorInput[0] == "changePolicy")
        {
            return new ChangePlanPolicy(std::stoi(vectorInput[1]), vectorInput[2]);
        }
    }
    if (vectorInput.size() == 7)
    {
        if (vectorInput[0] == "facility")
        {
            FacilityCategory facilType = {
                std::stoi(vectorInput[2]) == 0 ? FacilityCategory::LIFE_QUALITY : std::stoi(vectorInput[2]) == 1 ? FacilityCategory::ECONOMY
                                                                                                                 : FacilityCategory::ENVIRONMENT};
            return new AddFacility(vectorInput[1], facilType, std::stoi(vectorInput[3]), std::stoi(vectorInput[4]), std::stoi(vectorInput[5]), std::stoi(vectorInput[6]));
        }
    }
    return nullptr;
}

void Simulation ::start()
{
    open();
    cout << "The simulation has started" << endl;
    while (isRunning)
    {
        string input;
        cin >> input;
        vector<std::string> vectorInput = Auxiliary::parseArguments(input);
        if (vectorInput.size() && vectorInput[0] == "close")
        {
            close();
        }
        else
        {
            BaseAction *action = navigateAction(vectorInput);
            if (action != nullptr)
            {
                (*action).act(*this);
                if ((*action).getStatus() == ActionStatus::ERROR)
                {
                    cout << action->stringErrorMsg() << endl;
                }
                addAction(action);
            }
            else
            {
                cout << "INVALID ACTION" << endl;
            }
        }
    }
}

bool Simulation::addFacility(FacilityType facility)
{
    for (FacilityType ft : facilitiesOptions)
    {
        if (ft.getName() == facility.getName())
        {
            return false;
        }
    }
    facilitiesOptions.push_back(facility);
    return true;
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

bool Simulation::isValidPlan(int id)
{
    if (id - 1 >= 0 && id - 1 < plans.size())
    {
        return true;
    }
    return false;
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

SelectionPolicy *Simulation ::createSelectionPolicy(const string &policyName)
{
    SelectionPolicy *sp; /*DELETE?*/
    if (policyName == "nve")
    {
        sp = new NaiveSelection();
    }
    else if (policyName == "bal")
    {
        sp = new BalancedSelection(0, 0, 0);
    }
    else
    {
        sp = new EconomySelection();
    }
    return sp;
}