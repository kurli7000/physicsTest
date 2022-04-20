#ifndef Simulation_hpp
#define Simulation_hpp

#include <stdio.h>
#include <ctime>
#include <vector>
#include <list>
#include <chrono>
#include <iostream>
#include "Physics.hpp"
#include "Command.hpp"

class Simulation
{
public:
    Simulation(std::string name);
    ~Simulation();
    void Tick();
    void Init();
    std::vector<Unit*>* getUnits() { return &units; }
    std::list<Command*>* getCommands() { return &commands; }
    int getTick() { return lastTick; }
    void Rollback(int toTick);
    bool isRollingBack() { return rollbackMode; }
    float getStableMs() { return stableMs; }
    
private:
    struct Snapshot
    {
        int tick;
        std::vector<Unit*>* units;
        Snapshot(int lastTick, std::vector<Unit*>* backup) : tick(lastTick), units(backup) {};
    };
    
    const int millisecondsPerTick = 33;
    const int snapshotInterval = 40;
    const int maxTicksPerFrame = 3;
    std::chrono::steady_clock::time_point startTime;
    int getMs();
    int lastTick;
    std::string debugName;
    std::vector<Unit*> units;
    std::list<Command*> commands;
    std::list<Command*>::iterator commandIterator;
    std::list<Snapshot> snapshots;
    bool rollbackMode;
    float stableMs;
    static const int stableMsSamples = 32;
    int msSamples[stableMsSamples];
    int samplePos = 0;

    void ProcessUnits();
    void RunCommands(int tick);
    void TakeSnapshot();
};

#endif
