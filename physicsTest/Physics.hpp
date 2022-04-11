#ifndef Physics_hpp
#define Physics_hpp

#include "Vec.h"
#include <math.h>
#include <vector>

class Unit
{
public:
    Unit() {};
    ~Unit() {};
    Vec m_Pos;
    Vec m_Velocity;
    int m_Radius;
    int m_Mass;
    int m_OrderNumber;
};

struct Manifold
{
    Unit* A;
    Unit* B;
    int penetration;
    Vec normal;
};

void ResolveCollisions(std::vector<Unit*> units);

#endif /* Physics_hpp */
