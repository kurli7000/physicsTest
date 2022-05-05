#include "Simulation.hpp"
#include <iostream>

using namespace std;

Simulation::Simulation(string name) :
    lastTick(0),
    debugName(name),
    snapshots(66)
{
    memset(msSamples, 0, stableMsSamples * sizeof(int));
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

void Simulation::Tick()
{    
    int startMs = getMs();
    int tick = startMs / millisecondsPerTick;
    int perFrame = 0;
    rollbackMode = false;

    while (lastTick < tick)
    {
        if (lastTick % snapshotInterval == 0)
        {
            TakeSnapshot();
        }

        RunCommands(lastTick);
        ProcessUnits();
        lastTick++;
        perFrame++;
        
        if (perFrame >= maxTicksPerFrame)
        {
            rollbackMode = true;
            break;
        }
    }
    
    msSamples[samplePos] = getMs() - startMs;
    samplePos = (samplePos + 1) % stableMsSamples;
    
    stableMs = 0.0;
    maxMs = 0.0;
    for (int i = 0; i < stableMsSamples; i++)
    {
        if (msSamples[i] > maxMs) maxMs = msSamples[i];
        stableMs += msSamples[i] / (float)stableMsSamples;
    }
}

float Simulation::getFrameFraction()
{
    return float(getMs() - lastTick * millisecondsPerTick) / float(millisecondsPerTick);
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
    cout << debugName << ": " << "Snapshot at " << lastTick << endl;
}

void Simulation::Rollback(int toTick)
{
    assert(toTick >= 0 && toTick <= lastTick);
    
    cout << debugName << ": " << "Current tick: " << lastTick << ", rolling back to tick " << toTick << endl;

    // step 1 back to avoid using current head
    snapshots.backward();
    while (snapshots.back().tick > toTick)
    {
        cout << debugName << ": " << "   Snapshot rollback at " << snapshots.back().tick << endl;
        snapshots.back().Release();
        snapshots.backward();
    }

    // restore units
    lastTick = snapshots.back().tick;
    cout << debugName << ": " << "   Restoring snapshot " << lastTick << endl;
    Unit::ReplaceData(snapshots.back().units, &units);
    
    // back off command iterator
    while ((*commandIterator)->getTick() >= lastTick)
    {
        commandIterator--;
    }
    commandIterator++;
    cout << debugName << ": " << "   Command buffer at tick " << (*commandIterator)->getTick() << endl;
}

Simulation::~Simulation()
{
}

