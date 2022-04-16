#ifndef Simulation_hpp
#define Simulation_hpp

#include <stdio.h>
#include <ctime>
#include <vector>
#include "Physics.hpp"
#include <chrono>

class Simulation
{
public:
    Simulation();
    ~Simulation();
    bool Tick();
    std::vector<Unit*>* getUnits() { return &units; }
    
private:
    const int millisecondsPerTick = 33;
    std::chrono::steady_clock::time_point startTime;
    int getMs();
    int lastTick;
    std::vector<Unit*> units;
    
    void ProcessUnits(int pushRed);
};

#endif
