#include "Action.h"
#include <iostream>
using namespace std;
extern Simulation* backup = nullptr;

SimulateStep ::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){}
void SimulateStep ::act(Simulation &simulation)
{
    // how to change status?
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
}

// void SimulateStep::complete()
// {
//     (*this).status = ActionStatus::COMPLETED;
// }
const string SimulateStep::toString() const
{
    return "step" + std::to_string(numOfSteps);
}
SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

AddPlan ::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName), selectionPolicy(selectionPolicy) {}
void AddPlan::act(Simulation &simulation)
{
    if (!simulation.isSettlementExists(settlementName) || (selectionPolicy != "nve" && selectionPolicy != "bal" && selectionPolicy != "eco"))
    {
        error("Cannot create this plan");
    }
    else
    {
        simulation.addPlan(simulation.getSettlement(settlementName), simulation.createSelectionPolicy(selectionPolicy));
        complete();
    }
}
const string AddPlan::toString() const
{
    return "plan " + settlementName + " " + selectionPolicy;
}
AddPlan *AddPlan::clone() const
{
    return new AddPlan(*this);
}

AddSettlement ::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName), settlementType(settlementType) {}
void AddSettlement::act(Simulation &simulation)
{
    Settlement *stl = new Settlement(settlementName, settlementType);
    if (simulation.addSettlement(stl))
    {
        complete();
    }
    else
    {
        delete stl;
        error("Settlement already exists");
    }
}
const string AddSettlement::toString() const
{
    return "settlement " + settlementName + " " + std::to_string(static_cast<int>(settlementType));
}
AddSettlement *AddSettlement::clone() const
{
    return new AddSettlement(*this);
}

AddFacility ::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) : facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}
void AddFacility::act(Simulation &simulation)
{
    FacilityType ft(FacilityType(facilityName, facilityCategory, price, lifeQualityScore, economyScore, environmentScore));
    if (simulation.addFacility(ft))
    {
        complete();
    }
    else
    {
        error("Facility already exists");
    }
}

PrintPlanStatus ::PrintPlanStatus(int planId) : planId(planId) {}
void PrintPlanStatus::act(Simulation &simulation)
{
    if (!simulation.isValidPlan(planId))
    {
        error("Plan doesn't exist");
    }
    else
    {
        complete();
        string statusToPring = simulation.getPlan(planId).toString();
        cout << statusToPring << endl;
    }
}

ChangePlanPolicy ::ChangePlanPolicy(const int planId, const string &newPolicy) : planId(planId), newPolicy(newPolicy) {}
void ChangePlanPolicy::act(Simulation &simulation)
{
    if (!simulation.isValidPlan(planId) || simulation.getPlan(planId).getSelectionPolicyName() == newPolicy)
    {
        error("Cannot change selection policy");
    }
    else
    {
        complete();
        SelectionPolicy *sp;
        if (newPolicy == "nve")
        {
        }
        simulation.getPlan(planId).setSelectionPolicy(simulation.createSelectionPolicy(newPolicy));
    }
}






Close:: Close(){}

void Close::act(Simulation &simulation){
    simulation.close();
}
Close *Close::clone() const{
    return new Close(*this);
}


//BackupSimulation

void BackupSimulation::act(Simulation &simulation){
    backup->close(); // אולי לא צריך 
    backup = simulation; //  נממש בנאי ונשים כבנאי 
}

BackupSimulation *BackupSimulation ::clone() const{
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const{
    return "BackupSimulation";
}

RestoreSimulation::RestoreSimulation(){}

void RestoreSimulation:: act(Simulation &simulation){
    if(backup == nullptr){

    }
    else{
        simulation = backup;
    }

}

RestoreSimulation *RestoreSimulation:: clone() const{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation:: toString() const{
    return "RestoreSimulation";
}


