#ifndef Rendering_hpp
#define Rendering_hpp

#include <vector>
#include "Unit.hpp"

class Rendering
{
public:
    static void Render(std::vector<Unit*>* units1, std::vector<Unit*>* units2);
    
private:
    static const int NUM_SEGMENTS = 20;
    
    static void DrawCircle(float x, float y, float r);
    static void DrawScene(std::vector<Unit*>* units, float offset);
};

#endif /* Rendering_hpp */
