#include "Simulation.hpp"
#include <iostream>

using namespace std;

Simulation::Simulation(string name) :
    lastTick(0),
    debugName(name)
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
    int perFrame = 0;

    while (lastTick < tick && perFrame < maxTicksPerFrame)
    {
        if (lastTick % snapshotInterval == 0 && (snapshots.size() == 0 || snapshots.back().tick != lastTick))
        {
            TakeSnapshot();
        }

        RunCommands(lastTick);
        ProcessUnits();
        lastTick++;
        perFrame++;
    }
    
    return true;
}

void Simulation::RunCommands(int tick)
{
    while (commandIterator != commands.end() && (*commandIterator)->getTick() == tick)
    {
        Command* command = *commandIterator;
        command->Execute(&units);
        commandIterator++;
        cout << debugName << ": " << "Executing command at tick " << command->getTick() << endl;
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
    
    Physics::ResolveCollisions(&units);
}

void Simulation::TakeSnapshot()
{
    vector<Unit*>* backup = new vector<Unit*>();
    Unit::CopyUnits(&units, backup);
    Snapshot snapshot(lastTick, backup);
    snapshots.push_back(snapshot);
    cout << debugName << ": " << "Snapshot at " << lastTick << ", size: " << snapshots.size() << endl;
}

void Simulation::Rollback(int toTick)
{
    assert(toTick >= 0 && toTick < lastTick);
    
    cout << debugName << ": " << "Current tick: " << lastTick << ", rolling back to tick " << toTick << endl;
    cout << debugName << ": " << "   Snapshots: " << snapshots.size() << endl;
 
    // find snapshot
    while (snapshots.back().tick > toTick)
    {
        cout << debugName << ": " << "   Discarding snapshot " << snapshots.back().tick << endl;
        (*snapshots.back().units).clear();
        snapshots.pop_back();
    }

    // restore units
    lastTick = snapshots.back().tick;
    cout << debugName << ": " << "   Restoring snapshot " << lastTick << endl;
    Unit::ReplaceData(snapshots.back().units, &units);
    
    // back off command iterator
    while ((*commandIterator)->getTick() >= lastTick)
    {
        commandIterator--;
        cout << debugName << ": " << "   Command buffer at tick " << (*commandIterator)->getTick() << endl;
    }
    // TODO fixme
    commandIterator++;
    cout << debugName << ": " << "   Command buffer at tick " << (*commandIterator)->getTick() << endl;
}

Simulation::~Simulation()
{
}

