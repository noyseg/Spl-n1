#include "Action.h"
#include <iostream>

using namespace std;

extern Simulation *backup;

//defualt constructor 
BaseAction::BaseAction():errorMsg(""),status(ActionStatus::COMPLETED) {}

ActionStatus BaseAction::getStatus() const{
    return status;
}

//set the Action status to complete when called
void BaseAction::complete()
{
    status = ActionStatus::COMPLETED;
}

//set Action error message to be as given
void BaseAction::error(string errorMsg)
{
    status = ActionStatus::ERROR;
    (*this).errorMsg = errorMsg;
};
//returning Action error message
const string &BaseAction::getErrorMsg() const
{
    return errorMsg;
};

//returning the action status as a string
const string BaseAction::getStatusString() const
{
    if (status == ActionStatus::ERROR)
    {
        return "ERROR";
    }
    return "COMPLETED";
}

//constructor for SimulateStep
SimulateStep ::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps){}

void SimulateStep ::act(Simulation &simulation)
{
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

//constructor for AddPlan
AddPlan ::AddPlan(const string &settlementName, const string &selectionPolicy) : settlementName(settlementName), selectionPolicy(selectionPolicy) {}

//Adding the given plan to the simulation if possible printing an error if not
void AddPlan::act(Simulation &simulation)
{
    if (!simulation.isSettlementExists(settlementName) || (selectionPolicy != "nve" && selectionPolicy != "bal" 
    && selectionPolicy != "eco" && selectionPolicy != "env"))
    {
        error("Cannot create this plan");
        cout << "Error:" + getErrorMsg() << endl;
    }
    else
    {
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

//constructor for AddSettlement
AddSettlement ::AddSettlement(const string &settlementName, SettlementType settlementType) : settlementName(settlementName), settlementType(settlementType) {}

//Adding the given settlement to the simulation if possible printing an error if not
void AddSettlement::act(Simulation &simulation)
{
    Settlement *stl = new Settlement(settlementName, settlementType);
    if (simulation.addSettlement(stl))//checking settlement validity, simulation will handle the pointer deletion 
    {
        complete();
    }
    else
    {
        delete stl;
        stl = nullptr;
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

//constructor for AddFacility
AddFacility ::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore) : facilityName(facilityName), facilityCategory(facilityCategory), price(price), lifeQualityScore(lifeQualityScore), economyScore(economyScore), environmentScore(environmentScore) {}

//Adding the given facility to the simulation if possible printing an error if not
void AddFacility::act(Simulation &simulation)
{
    //creating facility type based on the params given in the constructor
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

//constructor for PrintPlanStatus
PrintPlanStatus ::PrintPlanStatus(int planId) : planId(planId) {}

//printing the given plan by the id if possible, otherwise printing an error 
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
        simulation.getPlan(planId).printStatus();//printing using Plan function
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

//constructor for ChangePlanPolicy
ChangePlanPolicy ::ChangePlanPolicy(const int planId, const string &newPolicy) : planId(planId), newPolicy(newPolicy) {}

//changing the current policy of the plan given by the id in the constructor
void ChangePlanPolicy::act(Simulation &simulation)
{
    //cannot change policy to the current one
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
        // setSelectionPolicy function will steal the resources and handle the deletion current selection policy 
        int lifeQualityScore = plan.getlifeQualityScore();
        int economyScore = plan.getEconomyScore();
        int environmentScore = plan.getEnvironmentScore();
        for(Facility *underConstructio : plan.getUnderConstruction()){
                lifeQualityScore += underConstructio->getLifeQualityScore();
                economyScore += underConstructio->getEconomyScore();
                environmentScore += underConstructio->getEnvironmentScore();
        }
        SelectionPolicy *sp = simulation.createSelectionPolicy(newPolicy, lifeQualityScore, economyScore, environmentScore);
        simulation.getPlan(planId).setSelectionPolicy(sp);
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

// Defualt constructor for PrintActionsLog
PrintActionsLog ::PrintActionsLog() {}

// Printing each action in the action log of the simulation
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

//defualt constructor for Close
Close::Close() {}

//closing the simualtion by calling close function in the simulation
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

// constructor for BackupSimulation
BackupSimulation::BackupSimulation() {}

//performing a backup to the current simulation
void BackupSimulation::act(Simulation &simulation)
{
    if (backup == nullptr)
    {
        backup = new Simulation(simulation); //allocate a new simulation
    }
    else
    {
        *backup = simulation; //overwrite the existing object
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

// Constructor for RestoreSimulation
RestoreSimulation::RestoreSimulation() {}

// Performing a restore to the current simulation if a backup was executed before 
void RestoreSimulation::act(Simulation &simulation)
{
    if (backup == nullptr)
    {
        error("No backup available");
        cout << "ERROR:" << getErrorMsg() << endl;
    }
    else
    {
        simulation = *backup; // Using operator=(const Simulation &otherSimulation) in simulation
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
