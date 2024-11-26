
#include "SelectionPolicy.h"


NaiveSelection::NaiveSelection(): lastSelectedIndex(-1){}

// Assume facilities Options is not empty 
const FacilityType& NaiveSelection::selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}

// to fill
const string NaiveSelection::toString() const {

}

NaiveSelection *NaiveSelection::clone() const {
    return new NaiveSelection(*this);
}
