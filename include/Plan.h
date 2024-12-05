#pragma once
#include <vector>
#include "Facility.h"
#include "Settlement.h"
#include "SelectionPolicy.h"
using std::vector;

enum class PlanStatus {
    AVALIABLE,
    BUSY,
};

class Plan {
    public:
        Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions);
        Plan(const Plan &otherPlan); //copy constructor
        Plan(const Plan &otherPlan,Settlement &settlement); //semi copy constructor 
        Plan(Plan &&otherPlan); //move constructor 
        ~Plan(); // Distructor
        Plan& operator=(const Plan& other) = delete; // Disabled assignment opertaor
        Plan& operator=(Plan&& other) = delete; // Disabled move assignment opertaor
        const int getlifeQualityScore() const;
        const int getEconomyScore() const;
        const int getEnvironmentScore() const;
        const string getSelectionPolicyName()const;
        void setSelectionPolicy(SelectionPolicy *selectionPolicy);
        void step();
        const string statusToString() const;
        void printStatus();
        const vector<Facility*> &getFacilities() const;
        void addFacility(Facility* facility);
        const string toString() const;
        const string getPlanSettlement() const;
        void printPlanValuesSummery();
        const vector<Facility*> &getUnderConstruction() const;

    private:
        int plan_id;
        const Settlement &settlement;
        SelectionPolicy *selectionPolicy; //What happens if we change this to a reference?
        PlanStatus status;
        vector<Facility*> facilities;
        vector<Facility*> underConstruction;
        const vector<FacilityType> &facilityOptions;
        int life_quality_score, economy_score, environment_score;
};