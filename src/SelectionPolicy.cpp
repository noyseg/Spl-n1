
#include "SelectionPolicy.h"
#include <iostream>
#include <algorithm> // for min and max functions 

using namespace std;

// Constructor of NaiveSelection
NaiveSelection::NaiveSelection(): lastSelectedIndex(-1){}

// Returns the next FacilityType that is selected by NaiveSelection
// Assume facilities Options is not empty
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

// Retruns abbreviated name of Naive selection policy
const string NaiveSelection::toString() const {
    return "nve";
}

// Returns current NaiveSelection's clone 
// Uses defult constructor of NaiveSelection
NaiveSelection *NaiveSelection::clone() const{
    return new NaiveSelection(*this);
}

// BalancedSelection - selection that choose the facility that by adding plan scores will be closest to balance 

// Constructor of BalancedSelection 
BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore){}

// Updated current scores with plan scores 
void BalancedSelection::setScores(int addLifeQualityScore, int addEconomyScore, int addEnvironmentScore){
    LifeQualityScore += addLifeQualityScore;
    EconomyScore += addEconomyScore;
    EnvironmentScore += addEnvironmentScore;
}

// void BalancedSelection::printScores(){
//     cout << "L" << LifeQualityScore << endl;
//     cout <<  "E" <<EconomyScore << endl;
//     cout <<  "En" << EnvironmentScore << endl;
// }

// Returns the distance between the maximun value that will be made by adding plan scores to the minimum value that will be made by adding plan scores
int BalancedSelection::distance(int sumLifeQualityScore, int sumEconomyScore, int sumEnvironmentScore){
     int maximum = std::max({sumLifeQualityScore,sumEconomyScore,sumEnvironmentScore});
     int minimum = std::min({sumLifeQualityScore,sumEconomyScore,sumEnvironmentScore});
     return maximum - minimum;
}

// Returns the next FacilityType that is selected by BalancedSelection
// Assume facilities Options is not empty
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    int minI = 0; 
    int minDistance = distance(LifeQualityScore+facilitiesOptions[0].getLifeQualityScore(),EconomyScore+facilitiesOptions[0].getEconomyScore(),EnvironmentScore + facilitiesOptions[0].getEnvironmentScore());
    // if minDistance == 0 we need to take the first one that was checked 
    if(minDistance != 0){
        for (int i=1; i<facilitiesOptions.size();i++){
            int optionalMinDistance = distance(LifeQualityScore+facilitiesOptions[i].getLifeQualityScore(),EconomyScore+facilitiesOptions[i].getEconomyScore(),EnvironmentScore + facilitiesOptions[i].getEnvironmentScore());
            if (optionalMinDistance < minDistance){
                minDistance = optionalMinDistance;
                minI = i;
                // minDistance == 0 We need to take the the earilest one that was checked with minDistance == 0
                if(minDistance == 0){
                    setScores(facilitiesOptions[minI].getLifeQualityScore(),facilitiesOptions[minI].getEconomyScore(),facilitiesOptions[minI].getEnvironmentScore());
                    return facilitiesOptions[minI];
                }
            }
        }
    }
    setScores(facilitiesOptions[minI].getLifeQualityScore(),facilitiesOptions[minI].getEconomyScore(),facilitiesOptions[minI].getEnvironmentScore());
    return facilitiesOptions[minI];
}

// Retruns abbreviated name of Balanced selection policy
const string BalancedSelection::toString() const{
    return "bal";
}

// Returns current BalancedSelection's clone 
// Uses defult constructor of BalancedSelection
BalancedSelection *BalancedSelection::clone() const{
    return new BalancedSelection(*this);
}

// EconomySelection - selection by Economy Category 

// Constructor of EconomySelection
EconomySelection::EconomySelection(): lastSelectedIndex(-1){}

// Returns the next FacilityType that is selected by EconomySelection
// Assume facilities Options is not empty
const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    while(facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ECONOMY){
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    }
    return facilitiesOptions[lastSelectedIndex];
}

// Retruns abbreviated name of Economy selection policy
const string EconomySelection::toString() const {
    return "eco";
}

// Returns current EconomySelection's clone 
// Uses defult constructor of EconomySelection
EconomySelection *EconomySelection::clone() const{
    return new EconomySelection(*this);
}

// SustainabilitySelection - selection by enviroment Category 

// Constructor of SustainabilitySelection
SustainabilitySelection::SustainabilitySelection(): lastSelectedIndex(-1){}

// Returns the next FacilityType that is selected by SustainabilitySelection
// Assume facilities Options is not empty
const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    while(facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ENVIRONMENT){
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    }
    return facilitiesOptions[lastSelectedIndex];
}

// Retruns abbreviated name of Sustainability selection policy
const string SustainabilitySelection::toString() const {
    return "env";
}

// Returns current SustainabilitySelection's clone 
// Uses defult constructor of SustainabilitySelection
SustainabilitySelection *SustainabilitySelection::clone() const{
    return new SustainabilitySelection(*this);
}
