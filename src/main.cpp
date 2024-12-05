#include "Simulation.h"
#include <iostream>
#include <string>

using namespace std;

Simulation *backup = nullptr;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout << "usage: simulation <config_path>" << endl;
        return 0;
    }

    string configurationFile = argv[1];
    Simulation simulation(configurationFile);
    simulation.start();
    // Simulation preformed backup and we need to delete global object 
    if (backup != nullptr)
    {
        delete backup;
        backup = nullptr;
    }
    return 0;
}