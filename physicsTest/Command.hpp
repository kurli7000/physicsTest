#ifndef Command_hpp
#define Command_hpp

#include <stdio.h>
#include "Unit.hpp"
#include "Vec.h"

class Command
{
public:
    Command(int unit, int tick, Vec velocityIncrease);
    void Execute(std::vector<Unit*>* units);
    int getTick() { return tick; }
    
private:
    int unit;
    int tick;
    Vec velocity;
};

#endif /* Command_hpp */
