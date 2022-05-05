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

void Simulation::init()
{
    commandIterator = commands.begin();
}

void Simulation::update(int ms)
{    
    auto startTime = chrono::steady_clock::now();
    int tick = ms / millisecondsPerTick;
    int perFrame = 0;
    rollbackMode = false;

    while (lastTick < tick)
    {
        if (lastTick % snapshotInterval == 0)
        {
            takeSnapshot();
        }

        runCommands(lastTick);
        processUnits();
        lastTick++;
        perFrame++;
        
        if (perFrame >= maxTicksPerFrame)
        {
            rollbackMode = true;
            break;
        }
    }
    
    // calculate durations
    auto now = chrono::steady_clock::now();
    int durationMs = (int)chrono::duration_cast<chrono::milliseconds>(now - startTime).count();
    msSamples[samplePos] = durationMs;
    samplePos = (samplePos + 1) % stableMsSamples;
    
    stableMs = 0.0;
    maxMs = 0.0;
    for (int i = 0; i < stableMsSamples; i++)
    {
        if (msSamples[i] > maxMs) maxMs = msSamples[i];
        stableMs += msSamples[i] / (float)stableMsSamples;
    }
}

float Simulation::getFrameFraction(int ms)
{
    return float(ms - lastTick * millisecondsPerTick) / float(millisecondsPerTick);
}

void Simulation::runCommands(int tick)
{
    while (commandIterator != commands.end() && (*commandIterator)->getTick() == tick)
    {
        Command* command = *commandIterator;
        command->execute(&units);
        commandIterator++;
        cout << debugName << ": " << "Executing command at tick " << command->getTick() << endl;
    }
}

void Simulation::processUnits()
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
    
    Physics::resolveCollisions(&units);
}

void Simulation::takeSnapshot()
{
    vector<Unit*>* backup = new vector<Unit*>();
    Unit::copyUnits(&units, backup);
    Snapshot snapshot(lastTick, backup);
    snapshots.push_back(snapshot);
    cout << debugName << ": " << "Snapshot at " << lastTick << endl;
}

void Simulation::rollback(int toTick)
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
    Unit::replaceData(snapshots.back().units, &units);
    
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

