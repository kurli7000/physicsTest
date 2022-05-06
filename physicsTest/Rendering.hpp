#ifndef Rendering_hpp
#define Rendering_hpp

#include <vector>
#include "Unit.hpp"
#include "Simulation.hpp"

class Rendering
{
public:
    Rendering(Simulation* sim1, Simulation* sim2) : simulation1(sim1), simulation2(sim2) {};
    void render(int ms);
    
private:
    static const int NUM_SEGMENTS = 20;
    
    void drawCircle(float x, float y, float r, bool distort);
    void drawScene(Simulation* simulation, float offset);
    static float distortX(float y, float t);
    
    Simulation* simulation1;
    Simulation* simulation2;
    int timer;
};

#endif /* Rendering_hpp */
