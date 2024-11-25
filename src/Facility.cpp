#include "Facility.h"

// Cons
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) : name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score) {}












































// Constructor for Facility
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score):
FacilityType(name,category,price,lifeQuality_score,economy_score,environment_score),settlementName(settlementName),status(FacilityStatus::UNDER_CONSTRUCTIONS),timeLeft(price){}

// Constructor for Facility - gets FacilityType
Facility(const FacilityType &type, const string &settlementName):
name(type.getName()), category(type.getCategory()), price(type.getCost()), lifeQuality_score(lifeQuality_score), economy_score(type.getEconomyscore()), 
environment_score(type.getEnvironmentScore()), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS),timeLeft(type.getCost()){}

const string &getSettlementName() const;
const int getTimeLeft() const;
FacilityStatus step();
void setStatus(FacilityStatus status);
const FacilityStatus& getStatus() const;
const string toString() const;