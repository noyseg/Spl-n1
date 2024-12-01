#include "Action.h"
extern Simulation* backup = nullptr;

SimulateStep ::SimulateStep(const int numOfSteps): numOfSteps(numOfSteps){}
void SimulateStep ::act(Simulation &simulation)
{
    for (int i = 0; i < numOfSteps; i++)
    {
        simulation.step();
    }
}
const string SimulateStep::toString() const
{
    return "step" + std::to_string(numOfSteps);
}
SimulateStep *SimulateStep::clone() const{
    return new SimulateStep(*this);
}








































































































Close:: Close(){}

void Close::act(Simulation &simulation){
    simulation.close();
}
Close *Close::clone() const{
    return new Close(*this);
}


//BackupSimulation

void BackupSimulation::act(Simulation &simulation){
    backup->close(); // אולי לא צריך 
    backup = simulation; //  נממש בנאי ונשים כבנאי 
}

BackupSimulation *BackupSimulation ::clone() const{
    return new BackupSimulation(*this);
}

const string BackupSimulation::toString() const{
    return "BackupSimulation";
}

RestoreSimulation::RestoreSimulation(){}

void RestoreSimulation:: act(Simulation &simulation){
    if(backup == nullptr){

    }
    else{
        simulation = backup;
    }

}

RestoreSimulation *RestoreSimulation:: clone() const{
    return new RestoreSimulation(*this);
}

const string RestoreSimulation:: toString() const{
    return "RestoreSimulation";
}


