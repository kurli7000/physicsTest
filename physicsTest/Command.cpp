#include "Command.hpp"

Command::Command(Unit* unit, int tick, Vec velocityIncrease) :
    unit(unit),
    tick(tick),
    velocity(velocityIncrease)
{}

void Command::Execute()
{
    unit->IncreaseVelocity(velocity);
}
