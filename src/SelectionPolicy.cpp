
#include "SelectionPolicy.h"
#include <algorithm> // for min and max functions 

NaiveSelection::NaiveSelection(): lastSelectedIndex(-1){}

// Assume facilities Options is not empty 
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

// to fill
const string NaiveSelection::toString() const {
    return "nve";
}

NaiveSelection *NaiveSelection::clone() const{
    return new NaiveSelection(*this);
}

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore){}

int BalancedSelection::distance(int sumLifeQualityScore, int sumEconomyScore, int sumEnvironmentScore){
     int maximum = std::max({sumLifeQualityScore,sumEconomyScore,sumEnvironmentScore});
     int minimum = std::min({sumLifeQualityScore,sumEconomyScore,sumEnvironmentScore});
     return maximum - minimum;
}

// remember to check if we need to update life scores 
const FacilityType& BalancedSelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    int minI = 0; 
    int minDistance = distance(facilitiesOptions[0].getLifeQualityScore()+LifeQualityScore,facilitiesOptions[0].getEconomyScore()+EconomyScore,facilitiesOptions[0].getEnvironmentScore()+EnvironmentScore);
    if(minDistance == 0){
        return facilitiesOptions[minI];
    }
    for (int i=1; i<facilitiesOptions.size();i++){
        int optionalMinDistance = distance(facilitiesOptions[i].getLifeQualityScore()+LifeQualityScore,facilitiesOptions[i].getEconomyScore()+EconomyScore,facilitiesOptions[i].getEnvironmentScore()+EnvironmentScore);
        if (optionalMinDistance < minDistance){
            minDistance = optionalMinDistance;
            minI = i;
            if(minDistance == 0){
                return facilitiesOptions[minI];
            }
        }
    }
    return facilitiesOptions[minI];
}

//to fill
const string BalancedSelection::toString() const{
    return "bal";
}


BalancedSelection *BalancedSelection::clone() const{
    return new BalancedSelection(*this);
}




// EconomySelection - selection by Economy Category 
EconomySelection::EconomySelection(): lastSelectedIndex(-1){}

const FacilityType& EconomySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    while(facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ECONOMY){
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    }
    return facilitiesOptions[lastSelectedIndex];
}

// to fill
const string EconomySelection::toString() const {
    return "eco";
}

EconomySelection *EconomySelection::clone() const{
    return new EconomySelection(*this);
}

// SustainabilitySelection - selection by enviroment Category 
SustainabilitySelection::SustainabilitySelection(): lastSelectedIndex(-1){}
const FacilityType& SustainabilitySelection::selectFacility(const vector<FacilityType>& facilitiesOptions){
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    while(facilitiesOptions[lastSelectedIndex].getCategory() != FacilityCategory::ENVIRONMENT){
        lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    }
    return facilitiesOptions[lastSelectedIndex];
}

const string SustainabilitySelection::toString() const {
    return "env";
}

SustainabilitySelection *SustainabilitySelection::clone() const{
    return new SustainabilitySelection(*this);
}
