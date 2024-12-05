#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"

using std::string;
using std::vector;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        Simulation(const Simulation &otherSimulation);
        Simulation(Simulation &&otherSimulation);
        ~Simulation();
        Simulation &operator=(const Simulation &otherSimulation);
        Simulation &operator=(Simulation &&otherSimulation);
        BaseAction* navigateAction(vector<std::string> vectorInput);
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        vector<BaseAction*> getActionsLog()const;
        Plan &getPlan(const int planID);
        bool isValidPlan(int id);
        void step();
        void close();
        void open();
        SelectionPolicy *createSelectionPolicy(const string &policyName,int lifeQualityScore, int economyScore, int environmentScore);

private:
    bool isRunning;
    int planCounter; // For assigning unique plan IDs
    vector<BaseAction *> actionsLog;
    vector<Plan> plans;
    vector<Settlement *> settlements;
    vector<FacilityType> facilitiesOptions;
};