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
        Simulation(const string &configFilePath); // Constructor 
        Simulation(const Simulation &otherSimulation); // Copy constructor
        Simulation(Simulation &&otherSimulation); // Move constructor 
        ~Simulation(); // Distructor
        Simulation &operator=(const Simulation &otherSimulation); // Assignment opertaor
        Simulation &operator=(Simulation &&otherSimulation); // Move assignment opertaor
        BaseAction* navigateAction(vector<std::string> vectorInput); // 
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        vector<BaseAction*> getActionsLog() const; // For log action - in order to print each action 
        Plan &getPlan(const int planID);
        bool isValidPlan(int id); // Check if plan is in range of currnet plans
        void step();
        void close();
        void open();
        // Creates new selection policy according to input string, with given values
        SelectionPolicy *createSelectionPolicy(const string &policyName,int lifeQualityScore, int economyScore, int environmentScore); 

private:
    bool isRunning;
    int planCounter; // For assigning unique plan IDs
    vector<BaseAction *> actionsLog;
    vector<Plan> plans;
    vector<Settlement *> settlements;
    vector<FacilityType> facilitiesOptions;
};