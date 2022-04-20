#ifndef Rendering_hpp
#define Rendering_hpp

#include <vector>
#include "Unit.hpp"
#include "Simulation.hpp"

class Rendering
{
public:
    static void Render(Simulation* simulation1, Simulation* simulation2);
    
private:
    static const int NUM_SEGMENTS = 20;
    
    static void DrawCircle(float x, float y, float r);
    static void DrawScene(std::vector<Unit*>* units, float offset, bool rollback);
};

#endif /* Rendering_hpp */
