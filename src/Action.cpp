#include "Action.h"

SimulateStep ::SimulateStep(const int numOfSteps) : numOfSteps(numOfSteps) {}
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