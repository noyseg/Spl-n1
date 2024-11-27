#include "Plan.h"
using namespace std;

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions):
plan_id(plan_id),settlement(settlement),selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),facilities(),underConstruction(),
facilityOptions(facilityOptions),life_quality_score(0),economy_score(0),environment_score(0){}

Plan::Plan(const Plan &otherPlan):
    plan_id(otherPlan.plan_id),
    settlement(otherPlan.settlement),
    selectionPolicy((*(otherPlan.selectionPolicy)).clone()),
    status(otherPlan.status),
    facilities(),
    underConstruction(),
    facilityOptions(otherPlan.facilityOptions), //References cannot be reseated once initialized
    life_quality_score(otherPlan.life_quality_score),economy_score(otherPlan.economy_score),environment_score(otherPlan.environment_score){
    for (int i=0; i < facilities.size();i++){
        facilities.push_back(new Facility(*otherPlan.facilities[i])); //Uses defult copy constructor of Facility
    }
    for (int i=0; i < underConstruction.size();i++){
        facilities.push_back(new Facility(*otherPlan.underConstruction[i])); //Uses defult copy constructor of Facility
    }
    }

Plan::~Plan(){
    for (int i=0; i < facilities.size();i++){
        delete facilities[i];
    }
    for (int i=0; i < underConstruction.size();i++){
        delete underConstruction[i];
    }
}


const int Plan::getlifeQualityScore() const{
    return life_quality_score;
}

const int Plan::getEconomyScore() const{
    return economy_score;
}

const int Plan::getEnvironmentScore() const{
    return environment_score;
}

void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy){
    (*this).selectionPolicy = selectionPolicy;
}


// check same name
// check scores 
void Plan::step(){
    while(underConstruction.size() < static_cast<int>(settlement.getType())+1){
        // new and delete 
        Facility *newFacility = new Facility(selectionPolicy->selectFacility(facilityOptions), settlement.getName());
        underConstruction.push_back(newFacility);
    }
    for(int i=underConstruction.size(); i==0; i--){
        if (underConstruction[i]->step() == FacilityStatus::OPERATIONAL){
            facilities.push_back(underConstruction[i]);
            underConstruction.erase(underConstruction.begin()+i);
        }
    }

}



