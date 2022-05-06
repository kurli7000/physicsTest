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
#include "CircularBuffer.hpp"

class Simulation
{
public:
    Simulation(std::string name);
    ~Simulation();
    
    void update(int ms);
    
    std::vector<Unit*>* getUnits() { return &units; }
    int getTick() { return lastTick; }
    bool isRollingBack() { return rollbackMode; }
    float getStableMs() { return stableMs; }
    float getMaxMs() { return maxMs; }
    float getFrameFraction(int ms);
    int getNextCommandTick();
    void addCommand(Command* cmd);
    
    static const int MS_PER_TICK = 50;
    
private:
    struct Snapshot
    {
        int tick;
        std::vector<Unit*>* units;
        
        Snapshot(int lastTick, std::vector<Unit*>* backup) : tick(lastTick), units(backup) {};
        Snapshot() : tick(INT_MAX), units(nullptr) {};
        
        void Release()
        {
            tick = INT_MAX;
            delete units;
            units = nullptr;
        }
    };
    
    const int snapshotInterval = 40;
    const int maxTicksPerFrame = 2;
    int lastTick;
    std::string debugName;
    std::vector<Unit*> units;
    std::vector<Command*> commands;
    int commandPosition;
    CircularBuffer<Snapshot> snapshots;
    bool rollbackMode;
    float stableMs;
    float maxMs;
    static const int stableMsSamples = 32;
    int msSamples[stableMsSamples];
    int samplePos = 0;

    void rollback(int toTick);
    void processUnits();
    void runCommands(int tick);
    void takeSnapshot();
};

#endif
