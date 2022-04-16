#ifndef Command_hpp
#define Command_hpp

#include <stdio.h>
#include "Unit.hpp"
#include "Vec.h"

class Command
{
public:
    Command(Unit* unit, int tick, Vec velocityIncrease);
    void Execute();
    int GetTick() { return tick; }
    
private:
    Unit* unit;
    int tick;
    Vec velocity;
};

#endif /* Command_hpp */
