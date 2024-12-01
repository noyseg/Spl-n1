#include "Simulation.h"
#include "Settlement.cpp"
#include <iostream>
#include "Auxiliary.cpp"
#include "Action.cpp"
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
        else if (read[0] == "plan"){
            addPlan(getSettlement(read[1]),createSelectionPolicy(read[2],0,0,0));
        }
    }
}

// copy constructor 
Simulation ::Simulation(const Simulation &otherSimulation): isRunning(otherSimulation.isRunning),
planCounter(otherSimulation.planCounter), 
actionsLog(), plans(), settlements(), facilitiesOptions(otherSimulation.facilitiesOptions){
    for (Settlement *stl: otherSimulation.settlements){
        settlements.push_back(new Settlement(*stl));
    }
    for (Plan pl: otherSimulation.plans){
        Plan newPlan = Plan(pl);
    }
    for (BaseAction *act: otherSimulation.actionsLog){
        actionsLog.push_back(act->clone());
    }
}

// move constructor 
Simulation::Simulation(const Simulation &&otherSimulation):
isRunning(otherSimulation.isRunning),
planCounter(otherSimulation.planCounter),
actionsLog(std::move(otherSimulation.actionsLog)),
plans(std::move(otherSimulation.plans)),
settlements(std::move(otherSimulation.settlements)),
facilitiesOptions(std::move(otherSimulation.facilitiesOptions)) {
for (Settlement *stl: otherSimulation.settlements){
    stl = nullptr;
}
for (BaseAction *act: otherSimulation.actionsLog){
    act = nullptr;
}
}

// distructor 
Simulation::~Simulation(){
    for (BaseAction* action : actionsLog) {
        if (action){
            delete action; 
        }
    }
    for (Settlement* settlement : settlements) {
        if(settlement){
            delete settlement; 
        }
    }
}

// operator assigment 
Simulation &Simulation::operator=(const Simulation &otherSimulation){
    if (this != &otherSimulation) {
        isRunning = otherSimulation.isRunning;
        planCounter = otherSimulation.planCounter;
        for (BaseAction* action : actionsLog) {
            delete action;
        }
        actionsLog = otherSimulation.actionsLog;
        for (Settlement* settlement : settlements) {
            delete settlement;
        }
        settlements = otherSimulation.settlements;
        plans = otherSimulation.plans;
        facilitiesOptions = otherSimulation.facilitiesOptions;
    }
    return *this;
}

// move operator assigment 
Simulation &Simulation::operator=(Simulation &&otherSimulation){
    isRunning = otherSimulation.isRunning;
    planCounter = otherSimulation.planCounter;
    for (BaseAction* action : actionsLog) {
        if (action){
            delete action;
        }
    }
    for (BaseAction* action : actionsLog) {
        action = nullptr;
    }
    for (Settlement* settlement : settlements) {
        if (settlement){
            delete settlement;
        }
    }
    actionsLog = std::move(otherSimulation.actionsLog);
    for (Settlement* settlement : settlements) {
        settlement = nullptr;
    }
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
        string input;
        cin >> input;
        vector<std::string> vectorInput = Auxiliary::parseArguments(input);
        BaseAction *action = navigateAction(vectorInput);
        if (action != nullptr)
        {
            (*action).act(*this);
            addAction(action);
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

vector<BaseAction *> Simulation::getActionsLog()const
{
    return actionsLog;
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

