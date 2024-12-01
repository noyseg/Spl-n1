#include "Facility.h"
#include <iostream>
using namespace std;
// Constructor for FacilityType
FacilityType::FacilityType(const string &name, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score)
    : name(name), category(category), price(price), lifeQuality_score(lifeQuality_score), economy_score(economy_score), environment_score(environment_score) {}

// Returns facilityType's name
const string &FacilityType::getName() const
{
    return name;
}

// Returns facilityType's cost(price)
int FacilityType::getCost() const
{
    return price;
}

// Returns facilityType's Life Quality Score
int FacilityType::getLifeQualityScore() const
{
    return lifeQuality_score;
}

// Returns facilityType's Environment Score
int FacilityType::getEnvironmentScore() const
{
    return environment_score;
}

// Returns facilityType's Economy Score
int FacilityType::getEconomyScore() const
{
    return economy_score;
}

// Returns facilityType's category
FacilityCategory FacilityType::getCategory() const
{
    return category;
}

// Constructor for Facility
Facility::Facility(const string &name, const string &settlementName, const FacilityCategory category, const int price, const int lifeQuality_score, const int economy_score, const int environment_score) : FacilityType(name, category, price, lifeQuality_score, economy_score, environment_score), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(price) {}

// Constructor for Facility using given FacilityType
Facility::Facility(const FacilityType &type, const string &settlementName) : // name(type.getName()), category(type.getCategory()), price(type.getCost()), lifeQuality_score(lifeQuality_score), economy_score(type.getEconomyscore()),
                                                                             // environment_score(type.getEnvironmentScore()),
                                                                             FacilityType(type), settlementName(settlementName), status(FacilityStatus::UNDER_CONSTRUCTIONS), timeLeft(type.getCost())
{
}

// Returns facility's Settlement Name
const string &Facility::getSettlementName() const
{
    return settlementName;
}

// Returns the time that left for constructoring the facility
const int Facility::getTimeLeft() const
{
    return timeLeft;
}

// לוודא שבסימולציה לא מריצים צעד כאשר הזמן שנותר למפעל הוא 0
// Promote facility constructoring by one and return Facility Status after promoting
FacilityStatus Facility::step()
{
    if (status == FacilityStatus::UNDER_CONSTRUCTIONS)
    {
        timeLeft -= 1;

        if (timeLeft == 0)
        {
            setStatus(FacilityStatus::OPERATIONAL);
        }
    }
    return status;
}

// Setting facility's constructing staus
void Facility::setStatus(FacilityStatus status)
{
    (*this).status = status;
}

// Returns facility's constructing staus
const FacilityStatus &Facility::getStatus() const
{
    return status;
}

// Returns facility relevent data as a string
const string Facility::toString() const
{
    string categoryValue;
    if (category == FacilityCategory::LIFE_QUALITY)
    {
        categoryValue = "0";
    }
    else if (category == FacilityCategory::ECONOMY)
    {
        categoryValue = "1";
    }
    else
    {
        categoryValue = "2";
    }
    // Use stringstream to concatenate the facility relevent data
    std::stringstream ss;
    ss << "Facility " << name << " "
       << categoryValue << " "
       << std::to_string(price) << " "
       << std::to_string(lifeQuality_score) << " "
       << std::to_string(economy_score) << " "
       << std::to_string(environment_score);

    return ss.str();
}

FacilityCategory FacilityType:: stringToFacilityCategory(const string& category){
    if (category == "0")
    {
        return FacilityCategory::LIFE_QUALITY;
    }
    else if (category == "1")
    {
       return FacilityCategory::ECONOMY;
    }
    return FacilityCategory::ENVIRONMENT;

}
