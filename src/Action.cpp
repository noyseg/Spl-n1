#pragma once
#include "Action.h"
#include <iostream>
using namespace std;

extern Simulation *backup;

BaseAction::BaseAction() {}

void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    (*this).errorMsg = errorMsg;
};
const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
};
const string BaseAction::getStatusString() const
{
    if (status == ActionStatus::ERROR)
    {
        return "ERROR";
    }
    return "COMPLETED";
}

SimulateStep ::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps)
{
}
void SimulateStep ::act(Simulation &simulation)
{
    // how to change status?
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
    complete();
}

const string SimulateStep::toString() const
{
    return "step " + std::to_string(numOfSteps) + " " + getStatusString();
}
SimulateStep *SimulateStep::clone() const
{
    return new SimulateStep(*this);
}

AddPlan ::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName), selectionPolicy(selectionPolicy) {}
void AddPlan::act(Simulation &simulation)
{
    cout << simulation.isSettlementExists("anotherVillage") << endl;
    if (!simulation.isSettlementExists(settlementName) || (selectionPolicy != "nve" && selectionPolicy != "bal" && selectionPolicy != "eco"))
    {
        error("Cannot create this plan");
        cout << "Error:" + getErrorMsg() << endl;
    }
    else
    {
        cout << "adding Settlement = "+simulation.getSettlement(settlementName).toString()+" with policy="+selectionPolicy << endl;
        simulation.addPlan(simulation.getSettlement(settlementName), simulation.createSelectionPolicy(selectionPolicy, 0, 0, 0));
        complete();
    }
}
const string AddPlan::toString() const
{
    return "plan " + settlementName + " " + selectionPolicy + " " + getStatusString();
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
        cout << "Error:" << getErrorMsg() << endl;
    }
}
const string AddSettlement::toString() const
{
    return "settlement " + settlementName + " " + std::to_string(static_cast<int>(settlementType)) + " " + getStatusString();
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
        cout << "Error:" << getErrorMsg() << endl;
    }
}

AddFacility *AddFacility::clone() const
{
    return new AddFacility(*this);
}

const string AddFacility::toString() const
{
    return "facility " + facilityName + " " + std::to_string(static_cast<int>(facilityCategory)) + " " + std::to_string(price) + " " + std::to_string(lifeQualityScore) + " " + std::to_string(economyScore) + " " + std::to_string(environmentScore) + " " + getStatusString();
}

PrintPlanStatus ::PrintPlanStatus(int planId) : planId(planId) {}
void PrintPlanStatus::act(Simulation &simulation)
{
    if (!simulation.isValidPlan(planId))
    {
        error("Plan doesn't exist");
        cout << getErrorMsg() << endl;
        cout << "Error:" << getErrorMsg() << endl;
    }
    else
    {
        complete();
        simulation.getPlan(planId).printStatus();
    }
}

PrintPlanStatus *PrintPlanStatus::clone() const
{
    return new PrintPlanStatus(*this);
}

const string PrintPlanStatus::toString() const
{
    return "planStatus " + std::to_string(planId) + " " + getStatusString();
}

ChangePlanPolicy ::ChangePlanPolicy(const int planId, const string &newPolicy) : planId(planId), newPolicy(newPolicy) {}
void ChangePlanPolicy::act(Simulation &simulation)
{
    if (!simulation.isValidPlan(planId) || simulation.getPlan(planId).getSelectionPolicyName() == newPolicy)
    {
        error("Cannot change selection policy");
        cout << "Error:" << getErrorMsg() << endl;
    }
    else
    {
        complete();
        Plan plan(simulation.getPlan(planId));
        cout << "planID: " << planId << endl;
        cout << "previous policy: " << plan.getSelectionPolicyName() << endl;
        cout << "newPolicy: " << newPolicy << endl;
        plan.setSelectionPolicy(simulation.createSelectionPolicy(newPolicy, plan.getlifeQualityScore(), plan.getEconomyScore(), plan.getEnvironmentScore()));
    }
}

ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}
const string ChangePlanPolicy::toString() const
{
    return "changePolicy: " + std::to_string(planId) + " " + newPolicy + " " + getStatusString();
}

PrintActionsLog ::PrintActionsLog() {}
void PrintActionsLog::act(Simulation &simulation)
{
    for (BaseAction *bs : simulation.getActionsLog())
    {
        cout << bs->toString() << endl;
    }
    complete();
}
PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
const string PrintActionsLog::toString() const
{
    return "printActionLog " + getStatusString();
}

Close::Close() {}

void Close::act(Simulation &simulation)
{
    simulation.close();
}
Close *Close::clone() const
{
    return new Close(*this);
}
const string Close::toString() const
{
    return "closeSimulation";
}

// BackupSimulation

BackupSimulation::BackupSimulation() {}

void BackupSimulation::act(Simulation &simulation)
{
    if (backup == nullptr)
    {
        backup = new Simulation(std::move(simulation)); // Allocate a new Simulation
    }
    else
    {
        *backup = std::move(simulation); // Overwrite the existing object
    }
    complete();
}

BackupSimulation *BackupSimulation ::clone() const
{
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const
{
    return "backup " + getStatusString();
}

RestoreSimulation::RestoreSimulation() {}

void RestoreSimulation::act(Simulation &simulation)
{
    if (backup == nullptr)
    {
        error("No backup available");
        cout << "ERROR:" << getErrorMsg() << endl;
    }
    else
    {
        simulation = std::move(*backup);
        complete();
    }
}

RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation::toString() const
{
    return "RestoreSimulation " + getStatusString();
}
