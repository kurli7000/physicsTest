#ifndef Simulation_hpp
#define Simulation_hpp

#include <stdio.h>
#include <ctime>
#include <vector>
#include <list>
#include <chrono>
#include "Physics.hpp"
#include "Command.hpp"

class Simulation
{
public:
    Simulation();
    ~Simulation();
    bool Tick();
    void Init();
    std::vector<Unit*>* getUnits() { return &units; }
    std::list<Command*>* getCommands() { return &commands; }
    int getTick() { return lastTick; }
    
private:
    const int millisecondsPerTick = 17;
    std::chrono::steady_clock::time_point startTime;
    int getMs();
    int lastTick;
    std::vector<Unit*> units;
    std::list<Command*> commands;
    std::list<Command*>::iterator commandIterator;
    
    void ProcessUnits();
    void RunCommands(int tick);
};

#endif
