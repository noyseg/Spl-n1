#include "Simulation.h"
#include "Settlement.h"
#include <iostream>

using namespace std;

Simulation *backup = nullptr;

int main(int argc, char **argv)
{
    Settlement *stl = new Settlement("kFarSPL", SettlementType::VILLAGE);
    cout << stl->toString() << endl;
    Facility *facil = new Facility("kindergarden", "spl2", FacilityCategory::ECONOMY, 2, 2, 1, 1);
    cout << facil->toString() << endl;
    FacilityStatus fs(facil->step());
    if (fs == FacilityStatus::OPERATIONAL)
    {
        cout << "OPERATIONAL time left=" + std::to_string(facil->getTimeLeft()) << endl;
    }
    else
    {
        cout << "UNDER_CONSTRUCTION time left=" + std::to_string(facil->getTimeLeft()) << endl;
    }
    if (argc != 2)
    {
        cout << "usage: simulation <config_path>" << endl;
        return 0;
    }
    string configurationFile = argv[1];
    // Simulation simulation(configurationFile);
    // simulation.start();

    if (backup != nullptr)
    {
        delete backup;
        backup = nullptr;
    }
    return 0;
}