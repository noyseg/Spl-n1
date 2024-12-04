#include "Simulation.h"
#include "Settlement.h"
#include <iostream>
#include "Auxiliary.h"
#include "Action.h"
#include <fstream>
#include <string>
using namespace std;

// constructor
Simulation ::Simulation(const string &configFilePath) : isRunning(false), planCounter(0),
                                                        actionsLog(), plans(), settlements(), facilitiesOptions()
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
            FacilityType facil(FacilityType(read[1], FacilityType::stringToFacilityCategory(read[2]), std::stoi(read[3]), std::stoi(read[4]), std::stoi(read[5]), std::stoi(read[6])));
            addFacility(facil);
        }
        else if (read[0] == "plan")
        {
            addPlan(getSettlement(read[1]), createSelectionPolicy(read[2], 0, 0, 0));
        }
    }
}

// copy constructor
Simulation ::Simulation(const Simulation &otherSimulation) : isRunning(otherSimulation.isRunning),
                                                             planCounter(otherSimulation.planCounter),
                                                             actionsLog(), plans(), settlements(), facilitiesOptions(otherSimulation.facilitiesOptions)
{
    for (size_t i = 0; i < otherSimulation.settlements.size(); i++)
    {
        settlements.push_back(new Settlement(*(otherSimulation.settlements[i])));
    }
    for (size_t i = 0; i < otherSimulation.actionsLog.size(); i++)
    {
        actionsLog.push_back(otherSimulation.actionsLog[i]->clone());
    }
    for (size_t i = 0; i < otherSimulation.plans.size(); i++)
    {
        plans.push_back(Plan(otherSimulation.plans[i],getSettlement(otherSimulation.plans[i].getPlanSettlement())));
    }
}

// move constructor
Simulation::Simulation(Simulation &&otherSimulation) :isRunning(otherSimulation.isRunning),
                                                        planCounter(otherSimulation.planCounter),
                                                        actionsLog(std::move(otherSimulation.actionsLog)),
                                                        plans(std::move(otherSimulation.plans)),
                                                        settlements(std::move(otherSimulation.settlements)),
                                                        facilitiesOptions(std::move(otherSimulation.facilitiesOptions))
{
    otherSimulation.settlements.clear();
    otherSimulation.actionsLog.clear();
}

// distructor
Simulation::~Simulation()
{
    for (size_t i = 0; i < actionsLog.size(); i++){
        if (actionsLog[i]){
            delete actionsLog[i];
        }
    }
    actionsLog.clear();
    for (size_t i = 0; i < settlements.size(); i++){
        if (settlements[i]){
            delete settlements[i];
        }
    }
    settlements.clear();
}

// copy operator assigment
// Simulation &Simulation::operator=(const Simulation &otherSimulation)
// {
//     if (this != &otherSimulation)
//     {
//         isRunning = otherSimulation.isRunning;
//         planCounter = otherSimulation.planCounter;
//         // Clear existing actionsLog
//         for (size_t i = 0; i < actionsLog.size(); i++)
//         {
//             if (actionsLog[i]){
//                 delete actionsLog[i]; /// see if we have leak 
//             }
//         }
//         actionsLog.clear();
//         for (size_t i = 0; i < otherSimulation.actionsLog.size(); i++){
//             actionsLog.push_back(otherSimulation.actionsLog[i]->clone());
//         }
//         // Clear existing settlements
//         for (size_t i = 0; i < settlements.size(); i++){
//             const string myStlName = settlements[i]->getName();
//             bool settlementsInBackup = false;
//             for (size_t j = 0; settlementsInBackup == false & j < otherSimulation.settlements.size(); j++){
//                 if (otherSimulation.settlements[j]->getName() == myStlName){
//                     settlementsInBackup = true;
//                 }
//             }
//             if (!settlementsInBackup){
//                 if (settlements[i]){
//                     delete settlements[i];
//                 }
//             }
//         }
//         settlements.clear();
//         for (size_t i = 0; i < otherSimulation.settlements.size(); i++){
//             settlements.push_back(otherSimulation.settlements[i]); // Delete settlement if it's not in use
//         }
//         // Rebuild the plans vector
//         plans.clear();
//         // for (const Plan &plan : otherSimulation.plans)
//         // {
//         //     plans.push_back(plan);
//         // }
//         // here problem 
//         for (size_t i =0; i < otherSimulation.plans.size(); i++)
//         {
//             plans.push_back(Plan(otherSimulation.plans[i]));
//         }
//         facilitiesOptions.clear();
//         for (FacilityType ft : otherSimulation.facilitiesOptions)
//         {
//             facilitiesOptions.push_back(ft);
//         }
//     }
//     return *this;
// }

Simulation &Simulation::operator=(const Simulation &otherSimulation)
{
    if (this != &otherSimulation)
    {
        isRunning = otherSimulation.isRunning;
        planCounter = otherSimulation.planCounter;
        // Clear existing settlements
        for (size_t i = 0; i < settlements.size(); i++){
            if (settlements[i]){
                delete settlements[i];
             }
        }
        // Clear existing settlements
        settlements.clear();
        for (size_t i = 0; i < actionsLog.size(); i++){
             if (actionsLog[i]){
                delete actionsLog[i];
             }
        }
        actionsLog.clear();
        plans.clear();
        actionsLog.clear();
        facilitiesOptions.clear();
        for (FacilityType ft : otherSimulation.facilitiesOptions)
        {
            facilitiesOptions.push_back(ft);
        }
        for (size_t i = 0; i < otherSimulation.settlements.size(); i++) {
             settlements.push_back(new Settlement(*otherSimulation.settlements[i])); // Delete settlement if it's not in use
        }
        for (size_t i = 0; i < otherSimulation.actionsLog.size(); i++) {
             actionsLog.push_back(otherSimulation.actionsLog[i]->clone()); // Delete settlement if it's not in use
        }
        for (size_t i = 0; i < otherSimulation.plans.size(); i++)
        {
            plans.push_back(Plan(otherSimulation.plans[i],getSettlement(otherSimulation.plans[i].getPlanSettlement())));
        }
    }
    return *this;
}



// move operator assigment
Simulation &Simulation::operator=(Simulation &&otherSimulation)
{
    isRunning = otherSimulation.isRunning;
    planCounter = otherSimulation.planCounter;
    for (size_t i = 0; i < actionsLog.size(); i++)
    {
        if (actionsLog[i]){
            delete actionsLog[i]; /// see if we have leak 
        }
    }
    actionsLog.clear();
    actionsLog = std::move(otherSimulation.actionsLog);
    for (size_t i = 0; i < settlements.size(); i++){
        const string myStlName = settlements[i]->getName();
        bool settlementsInBackup = false;
        for (size_t j = 0; settlementsInBackup == false & j < otherSimulation.settlements.size(); j++){
            if (otherSimulation.settlements[j]->getName() == myStlName){
                settlementsInBackup = true;
            }
        }
        if (!settlementsInBackup){
            if (settlements[i]){
                delete settlements[i];
            }
        }
    }
    settlements.clear();
    settlements = std::move(otherSimulation.settlements);
    plans = std::move(otherSimulation.plans);
    facilitiesOptions = std::move(otherSimulation.facilitiesOptions);
    return *this;
}

BaseAction *Simulation::navigateAction(vector<std::string> vectorInput)
{
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
            // remeber to remove last resotre if needed
            return new RestoreSimulation();
        }
    }
    if (vectorInput.size() == 2)
    {
        // Print plan status
        // check is valid
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
    return nullptr;
}

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
            if (actionName.find("backup") == 0){ //document backup 
                addAction(action);
                (*action).act(*this);
            }
            else if (actionName.find("Restore") == 0){ //no document of restore 
                (*action).act(*this);
                if (action->getStatus() == ActionStatus::ERROR){
                    addAction(action);
                }
            }
            else{
                (*action).act(*this);
                addAction(action); //keep action with current status  
            }
        }
        else
        {
            cout << "INVALID ACTION" << endl;
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

// Assume settlement exist 
Settlement &Simulation ::getSettlement(const string &settlementName)
{
    for (size_t i=0; i < settlements.size(); i++){
        if (settlements[i]->getName() == settlementName)
        {
            return *settlements[i]; // Dereference pointer and return the object reference
        }
    }
    return *settlements[0]; // This line will never be called - chdfskdfsld,fdv
}

vector<BaseAction *> Simulation::getActionsLog() const
{
    return actionsLog;
}

Plan &Simulation ::getPlan(const int planId)
{
    // Assuming we checked for validity before ask for the plan 
    return plans[planId]; 
}

bool Simulation::isValidPlan(int id)
{
    if (id >= 0 && static_cast<size_t>(id) < plans.size()) //check for size t!!!!!!
    {
        return true;
    }
    return false;
}


void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    vector<FacilityType> &rFacilitiesOptions = facilitiesOptions;
    Plan p(planCounter, settlement, selectionPolicy, rFacilitiesOptions);
    plans.push_back(p);
    planCounter++;
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
    for (Plan &plan : plans)
    {
        plan.step();
    }
}

void Simulation::close()
{
    for (Plan &plan : plans)
    {
        plan.printStatus();
    }
    isRunning = false;
    for (size_t i = 0; i < actionsLog.size(); i++){
        if(actionsLog[i]){
            delete actionsLog[i];
        }
    }
    for (size_t i = 0; i < settlements.size(); i++){
    {
        if(settlements[i]){
            delete settlements[i];
        }
    }
    }
}

void Simulation ::open()
{
    isRunning = true;
}

SelectionPolicy *Simulation ::createSelectionPolicy(const string &policyName, int lifeQualityScore, int economyScore, int environmentScore)
{
    SelectionPolicy *sp; /*DELETE?*/
    if (policyName == "nve")
    {
        sp = new NaiveSelection();
    }
    else if (policyName == "bal")
    {
        sp = new BalancedSelection(lifeQualityScore, economyScore, environmentScore);
    }
    else if (policyName == "eco")
    {
        sp = new EconomySelection();
    }
    else
    {
        sp = new SustainabilitySelection();
    }
    return sp;
}
