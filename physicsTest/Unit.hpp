#ifndef Unit_hpp
#define Unit_hpp

#include <vector>
#include "Vec.h"

class Unit
{
public:
    Unit() {};
    ~Unit() {};
    Unit(const Unit &p1);
    Unit& operator = (const Unit& other);
    
    struct float2 {
        float x;
        float y;
        float2(float _x, float _y) : x(_x), y(_y) {};
    };

    float2 getRenderingPosition(float frameFraction);
    void increaseVelocity(Vec velocityIncrease);
    
    static void generateUnits(std::vector<Unit*>* units, int numUnits);
    static void copyUnits(std::vector<Unit*>* src, std::vector<Unit*>* dst);
    static void replaceData(std::vector<Unit*>* src, std::vector<Unit*>* dst);
    
    Vec pos;
    Vec velocity;
    int radius;
    int mass;
    int orderNumber;
    
private:
    static const int RADIUS = 600;
};

#endif /* Unit_hpp */
