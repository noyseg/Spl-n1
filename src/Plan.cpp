#include "Plan.h"
using namespace std;

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    :
plan_id(plan_id),settlement(settlement),selectionPolicy(selectionPolicy),status(PlanStatus::AVALIABLE),facilityOptions(facilityOptions),
facilities({}),life_quality_score(0),economy_score(0),environment_score(0){
    
}
