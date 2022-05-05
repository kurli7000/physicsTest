#include "Command.hpp"

using namespace std;

Command::Command(int unit, int tick, Vec velocityIncrease) :
    unit(unit),
    tick(tick),
    velocity(velocityIncrease)
{}

void Command::execute(vector<Unit*>* units)
{
    Unit* u = (*units)[unit];
    u->increaseVelocity(velocity);
}
