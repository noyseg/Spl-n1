#include "Simulation.h"
#include "Settlement.h"
#include <iostream>
#include "Auxiliary.h"
#include "Action.h"
#include <fstream> // Reading from config file
#include <string>
using namespace std;

// Constructor
// Assuming configFile input is valid, can be empty and with empty lines
Simulation ::Simulation(const string &configFilePath) : isRunning(false), planCounter(0),
                                                        actionsLog(), plans(), settlements(), facilitiesOptions()
{
    std::ifstream File(configFilePath);
    string line;
    while (getline(File, line))
    {
        // Skips empty lines
        if (line != "\r")
        {
            vector<std::string> read = Auxiliary::parseArguments(line);
            if (read[0] == "settlement")
            {
                Settlement *stl = new Settlement(read[1], Settlement::stringToSettlementType(read[2]));
                addSettlement(stl);
            }
            else if (read[0] == "facility")
            {
                FacilityType facil(FacilityType(read[1], FacilityType::stringToFacilityCategory(read[2]), std::stoi(read[3]), std::stoi(read[4]), std::stoi(read[5]), std::stoi(read[6])));
                addFacility(facil);
            }
            else if (read[0] == "plan")
            {
                // intilize selectionPolicy value's for bal, no actions were done and therefore values are zeros 
                addPlan(getSettlement(read[1]), createSelectionPolicy(read[2], 0, 0, 0));
            }
        }
    }
}

// Copy constructor
Simulation ::Simulation(const Simulation &otherSimulation) : isRunning(otherSimulation.isRunning),
                                                             planCounter(otherSimulation.planCounter),
                                                             actionsLog(), plans(), settlements(), facilitiesOptions(otherSimulation.facilitiesOptions)
{
    // Deep copies settlements
    for (size_t i = 0; i < otherSimulation.settlements.size(); i++)
    {
        settlements.push_back(new Settlement(*(otherSimulation.settlements[i])));
    }
    // Deep copies actions log
    for (size_t i = 0; i < otherSimulation.actionsLog.size(); i++)
    {
        actionsLog.push_back(otherSimulation.actionsLog[i]->clone());
    }
    // Deep copies plans with the new settlements we created
    for (size_t i = 0; i < otherSimulation.plans.size(); i++)
    {
        plans.push_back(Plan(otherSimulation.plans[i], getSettlement(otherSimulation.plans[i].getPlanSettlement())));
    }
}

// Move constructor
Simulation::Simulation(Simulation &&otherSimulation) : isRunning(otherSimulation.isRunning),
                                                       planCounter(otherSimulation.planCounter),
                                                       actionsLog(std::move(otherSimulation.actionsLog)),
                                                       plans(std::move(otherSimulation.plans)),
                                                       settlements(std::move(otherSimulation.settlements)),
                                                       facilitiesOptions(std::move(otherSimulation.facilitiesOptions))
{
    // Clears the moved stolen data beacuse rvalue, In order to prevent double deletion
    otherSimulation.settlements.clear();
    otherSimulation.actionsLog.clear();
}

// Distructor
Simulation::~Simulation()
{
    for (size_t i = 0; i < actionsLog.size(); i++)
    {
        if (actionsLog[i])
        {
            delete actionsLog[i];
        }
    }
    actionsLog.clear();
    for (size_t i = 0; i < settlements.size(); i++)
    {
        if (settlements[i])
        {
            delete settlements[i];
        }
    }
    settlements.clear();
}

// Assignment opertaor
Simulation &Simulation::operator=(const Simulation &otherSimulation)
{
    if (this != &otherSimulation)
    {
        isRunning = otherSimulation.isRunning;
        planCounter = otherSimulation.planCounter;
        // Clear existing settlements
        for (size_t i = 0; i < settlements.size(); i++)
        {
            if (settlements[i])
            {
                delete settlements[i];
            }
        }
        settlements.clear();
        // Clear existing actionsLog
        for (size_t i = 0; i < actionsLog.size(); i++)
        {
            if (actionsLog[i])
            {
                delete actionsLog[i];
            }
        }
        actionsLog.clear();
        plans.clear();
        facilitiesOptions.clear();
        // Deep copy of facilitiesOptions, settlements, actionsLog, plans
        for (FacilityType ft : otherSimulation.facilitiesOptions)
        {
            facilitiesOptions.push_back(ft);
        }
        for (size_t i = 0; i < otherSimulation.settlements.size(); i++)
        {
            settlements.push_back(new Settlement(*otherSimulation.settlements[i]));
        }
        for (size_t i = 0; i < otherSimulation.actionsLog.size(); i++)
        {
            actionsLog.push_back(otherSimulation.actionsLog[i]->clone());
        }
        for (size_t i = 0; i < otherSimulation.plans.size(); i++)
        {
            plans.push_back(Plan(otherSimulation.plans[i], getSettlement(otherSimulation.plans[i].getPlanSettlement())));
        }
    }
    return *this;
}

// Move assignment opertaor
Simulation &Simulation::operator=(Simulation &&otherSimulation)
{
    // Releasing old resources to prevent memory leaks
    isRunning = otherSimulation.isRunning;
    for (size_t i = 0; i < settlements.size(); i++)
    {
        if (settlements[i])
        {
            delete settlements[i];
        }
    }
    planCounter = otherSimulation.planCounter;
    for (size_t i = 0; i < actionsLog.size(); i++)
    {
        if (actionsLog[i])
        {
            delete actionsLog[i];
        }
    }
    actionsLog.clear();
    settlements.clear();
    // "Stealing" resources from the moved simulation
    actionsLog = std::move(otherSimulation.actionsLog);
    settlements = std::move(otherSimulation.settlements);
    plans = std::move(otherSimulation.plans);
    facilitiesOptions = std::move(otherSimulation.facilitiesOptions);
    return *this;
}

// Creates an appropriate action object based on user input
// VectorInput A vector of strings representing user input commands
// Returns A pointer to a BaseAction object, or nullptr if the input is invalid
BaseAction *Simulation::navigateAction(vector<std::string> vectorInput)
{
    // Navigate based on vector size first
    if (vectorInput.size() == 1)
    {
        if (vectorInput[0] == "close")
        {
            return new Close();
        }
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
            return new RestoreSimulation();
        }
    }
    if (vectorInput.size() == 2)
    {
        // Print plan status checks that plan_id is valid
        if (vectorInput[0] == "planStatus")
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
            SettlementType stlType = Settlement::stringToSettlementType(vectorInput[2]);
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
            FacilityCategory facilType = FacilityType::stringToFacilityCategory(vectorInput[2]);
            return new AddFacility(vectorInput[1], facilType, std::stoi(vectorInput[3]), std::stoi(vectorInput[4]), std::stoi(vectorInput[5]), std::stoi(vectorInput[6]));
        }
    }
    // No matched action was found
    return nullptr;
}

// Starts the simulation preforms actions based on user input,
void Simulation ::start()
{
    open();
    cout << "The simulation has started" << endl;
    while (isRunning)
    {
        string userInput;
        getline(std::cin, userInput);
        vector<std::string> vectorInput = Auxiliary::parseArguments(userInput);
        BaseAction *action = navigateAction(vectorInput);
        if (action != nullptr)
        {
            const string actionName = action->toString();
            if (actionName.find("backup") == 0)
            {   // In order to document backup
                addAction(action);
                (*action).act(*this);
            }
            else
            {
                (*action).act(*this);
                addAction(action); // Documant action with current status
            }
        }
        else
        {
            cout << "INVALID ACTION" << endl;
        }
    }
}

// Adds a facility to the simulation, return true if facility was not already simulation and therfore was added and false otherwise
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

// Returns true if settlement is already in simulation (by its name) and false otherwise
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

// Assume settlement exist
Settlement &Simulation ::getSettlement(const string &settlementName)
{
    for (size_t i = 0; i < settlements.size(); i++)
    {
        if (settlements[i]->getName() == settlementName)
        {
            return *settlements[i]; // Dereference pointer and return the object reference
        }
    }
    return *settlements[0]; // This line will never be called
}

// Returns actionLog
// For log action - in order to print each action
vector<BaseAction *> Simulation::getActionsLog() const
{
    return actionsLog;
}

// Returns a plan from plans with given planId
Plan &Simulation ::getPlan(const int planId)
{
    // Assuming we checked for validity before ask for the plan
    return plans[planId];
}

// Check if plan is in range of currnet plans
// Assume all plans id's are in sequence
bool Simulation::isValidPlan(int id)
{
    if (id >= 0 && static_cast<size_t>(id) < plans.size())
    {
        return true;
    }
    return false;
}

// Adds a plan to the simulation, keeps plans id's in a sequence
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    vector<FacilityType> &rFacilitiesOptions = facilitiesOptions;
    Plan p(planCounter, settlement, selectionPolicy, rFacilitiesOptions);
    plans.push_back(p);
    planCounter++;
}

// Adds an action to the actionsLog
void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}

// Adds a settlement to the simulation, return true if settelment is valid and therfore was added and false otherwise
bool Simulation::addSettlement(Settlement *settlement)
{
    string settlementName = settlement->getName();
    if (isSettlementExists(settlementName) == false)
    {
        settlements.push_back(settlement);
        return true;
    }
    else
    {
        // Prevent memory leak
        delete settlement;
    }
    return false;
}

// Promote the simulation by one step for each of the plans 
void Simulation::step()
{
    for (Plan &plan : plans)
    {
        plan.step();
    }
}

// Closes the simulation, prints all plan summery, clears from heap relevent data 
void Simulation::close()
{
    for (Plan &plan : plans)
    {
        plan.printPlanValuesSummery();
    }
    isRunning = false;
    // Releasing old resources to prevent memory leaks
    for (size_t i = 0; i < actionsLog.size(); i++)
    {
        if (actionsLog[i])
        {
            delete actionsLog[i];
        }
    }
    actionsLog.clear();
    for (size_t i = 0; i < settlements.size(); i++)
    {
        if (settlements[i])
        {
            delete settlements[i];
        }
    }
    settlements.clear();
}

// Sets simulation's status to be running 
void Simulation ::open()
{
    isRunning = true;
}

// Assume policyName is valisd
// Creates new selection policy according to input string, with given values
// If policyName is not "bal" we don't uses lifeQualityScore, economyScore and environmentScore
SelectionPolicy *Simulation ::createSelectionPolicy(const string &policyName, int lifeQualityScore, int economyScore, int environmentScore)
{
    if (policyName == "nve")
    {
        return new NaiveSelection();
    }
    else if (policyName == "bal")
    {
        return new BalancedSelection(lifeQualityScore, economyScore, environmentScore);
    }
    else if (policyName == "eco")
    {
        return new EconomySelection();
    }
    else // "env"
    {
        return new SustainabilitySelection();
    }
    return nullptr;
}
