#include "Plan.h"
#include <iostream>
#include <sstream> // For stringstream in toString

using namespace std;

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions) : plan_id(plan_id), settlement(settlement), selectionPolicy(selectionPolicy), status(PlanStatus::AVALIABLE), facilities(), underConstruction(),
                                                                                                                                            facilityOptions(facilityOptions), life_quality_score(0), economy_score(0), environment_score(0) {}

Plan::Plan(const Plan &otherPlan) : plan_id(otherPlan.plan_id),
                                    settlement(otherPlan.settlement),
                                    selectionPolicy((*(otherPlan.selectionPolicy)).clone()),
                                    status(otherPlan.status),
                                    facilities(),
                                    underConstruction(),
                                    facilityOptions(otherPlan.facilityOptions), // References cannot be reseated once initialized
                                    life_quality_score(otherPlan.life_quality_score), economy_score(otherPlan.economy_score), environment_score(otherPlan.environment_score)
{
    for (int i = 0; i < facilities.size(); i++)
    {
        facilities.push_back(new Facility(*otherPlan.facilities[i])); // Uses defult copy constructor of Facility
    }
    for (int i = 0; i < underConstruction.size(); i++)
    {
        underConstruction.push_back(new Facility(*otherPlan.underConstruction[i])); // Uses defult copy constructor of Facility
    }
}

// distructor
Plan::~Plan()
{
    for (int i = 0; i < facilities.size(); i++)
    {
        if (facilities[i])
        {
            delete facilities[i];
        }
    }
    for (int i = 0; i < underConstruction.size(); i++)
    {
        if (underConstruction[i])
        {
            delete underConstruction[i];
        }
    }
}

// move constructor
Plan::Plan(Plan &&otherPlan) : plan_id(otherPlan.plan_id),
                               settlement(otherPlan.settlement),
                               selectionPolicy(otherPlan.selectionPolicy),
                               status(otherPlan.status), facilities(otherPlan.facilities),
                               underConstruction(std::move(otherPlan.underConstruction)), 
                               facilityOptions(std::move(otherPlan.facilityOptions)),     // References cannot be reseated once initialized
                               life_quality_score(otherPlan.life_quality_score), economy_score(otherPlan.economy_score), environment_score(otherPlan.environment_score)
{
    otherPlan.selectionPolicy = nullptr;
    for (int i = 0; i < facilities.size(); i++)
    {
        otherPlan.underConstruction[i] = nullptr;
    }
    for (int i = 0; i < underConstruction.size(); i++)
    {
        otherPlan.underConstruction[i] = nullptr;
    }
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    (*this).selectionPolicy = selectionPolicy;
}

// check same name
// check scores
void Plan::step()
{
    while (statusToString() == "AVALIABLE")
    {
        // new and delete
        Facility *newFacility = new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName());
        addFacility(newFacility);
    }
    for (int i = underConstruction.size(); i == 0; i--)
    {
        if (underConstruction[i]->step() == FacilityStatus::OPERATIONAL)
        {
            facilities.push_back(underConstruction[i]);
            underConstruction.erase(underConstruction.begin() + i);
        }
    }
}

const string Plan::statusToString() const
{
    if (status == PlanStatus::AVALIABLE)
    {
        return "AVALIABLE";
    }
    return "BUSY";
}

void Plan::printStatus()
{
    cout << "Plan ID: " << plan_id << endl;
    cout << "Settlement Name: " + settlement.getName() << endl;
    cout << "planStatus: " + statusToString() << endl;
    cout << "selection Policy: " + (*selectionPolicy).toString() << endl;
    cout << "LifeQualityScore: " + life_quality_score << endl;
    cout << "EconomyScore: " + economy_score << endl;
    cout << "EnvironmentScore: " + environment_score << endl;
    for (int i = 0; i < facilities.size(); i++)
    {
        cout << "facilityName: " + facilities[i]->getName() << endl;
        cout << "facilityStatus: OPERATIONAL" << endl;
    }
    for (int i = 0; i < underConstruction.size(); i++)
    {
        cout << "facilityName: " + underConstruction[i]->getName() << endl;
        cout << "facilityStatus: UNDER_CONSTRUCTIONS" << endl;
    }
}

const vector<Facility *> &Plan::getFacilities() const
{
    return facilities;
}

void Plan::addFacility(Facility *facility)
{
    underConstruction.push_back(facility);
    if(underConstruction.size() == static_cast<int>(settlement.getType()) + 1){
        status = PlanStatus::AVALIABLE;
    }
}

const string Plan::toString() const
{
    std::stringstream ss;
    ss << plan_id;
    return ss.str();
}
