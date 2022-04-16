#include "Simulation.hpp"
#include <iostream>

using namespace std;

Simulation::Simulation() :
    lastTick(0)
{
}

void Simulation::Init()
{
    startTime = chrono::steady_clock::now();
    commandIterator = commands.begin();
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
        RunCommands(lastTick);
        ProcessUnits();
        lastTick++;
        processed = true;
    }
    
    return processed;
}

void Simulation::RunCommands(int tick)
{
    while (commandIterator != commands.end() && (*commandIterator)->GetTick() == tick)
    {
        Command* command = *commandIterator;
        command->Execute();
        commandIterator++;
    }
}

void Simulation::ProcessUnits()
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
    
    Physics::ResolveCollisions(units);
}


Simulation::~Simulation()
{
}

