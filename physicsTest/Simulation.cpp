#include "Simulation.hpp"
#include <iostream>

using namespace std;

Simulation::Simulation() : lastTick(0)
{
    startTime = chrono::steady_clock::now();
}

int Simulation::getMs()
{
    auto now = chrono::steady_clock::now();
    return (int)chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
}

bool Simulation::Tick()
{
    int tick = getMs() / millisecondsPerTick;
    bool processed = false;
    
    while (tick > lastTick)
    {
        bool push = false;
        if (lastTick % millisecondsPerTick == 0) push = true;
        ProcessUnits(push);
        lastTick++;
        processed = true;
    }
    return processed;
}

void Simulation::ProcessUnits(bool pushRed)
{
    for (auto unit : units)
    {
        unit->pos += unit->velocity;
        
        if (unit->pos.x < unit->radius)
        {
            unit->pos.x = unit->radius;
            unit->velocity.x = -unit->velocity.x;
        }
        
        if (unit->pos.x > 32 * Vec::SCALE - unit->radius)
        {
            unit->pos.x = 32 * Vec::SCALE - unit->radius;
            unit->velocity.x = -unit->velocity.x;
        }
        
        if (unit->pos.y < unit->radius)
        {
            unit->pos.y = unit->radius;
            unit->velocity.y = -unit->velocity.y;
        }
        
        if (unit->pos.y > 32 * Vec::SCALE - unit->radius)
        {
            unit->pos.y = 32 * Vec::SCALE - unit->radius;
            unit->velocity.y = -unit->velocity.y;
        }
        
        unit->velocity.y -= GRAVITY;
    }
    
    if (pushRed)
    {
        if (pushRed == 1) units[0]->velocity.x += 300;
        else units[0]->velocity.x -= 300;
        units[0]->velocity.y += 500;
    }

    Physics::ResolveCollisions(units);
}


Simulation::~Simulation()
{
}

