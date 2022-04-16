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
    
    Unit& operator = (const Unit& other)
    {
        pos = other.pos;
        velocity = other.velocity;
        radius = other.radius;
        mass = other.mass;
        orderNumber = other.orderNumber;
        return *this;
    }
    
    void IncreaseVelocity(Vec velocityIncrease);
    
    static void Precalc(std::vector<Unit*>* units, int numUnits);
    static void CopyUnits(std::vector<Unit*>* src, std::vector<Unit*>* dst);
    static void ReplaceData(std::vector<Unit*>* src, std::vector<Unit*>* dst);
    
    Vec pos;
    Vec velocity;
    int radius;
    int mass;
    int orderNumber;
    
private:
    static const int RADIUS = 600;
};

#endif /* Unit_hpp */
