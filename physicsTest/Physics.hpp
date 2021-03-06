#ifndef Physics_hpp
#define Physics_hpp

#include <math.h>
#include <vector>
#include "Vec.h"
#include "Unit.hpp"

#define GRAVITY 0

class Physics
{
public:
    struct Manifold
    {
        Unit* A;
        Unit* B;
        int penetration;
        Vec normal;
        Manifold(Unit* a, Unit *b) : A(a), B(b) {};
    };
    
    Physics() {};
    ~Physics() {};
    static void resolveCollisions(std::vector<Unit*>* units);
    
private:
    static bool createManifold(Manifold* m);
    static void resolveCollision(Unit* a, Unit* b);
    
    static const int penetrationAdjustment = 10;
};

#endif /* Physics_hpp */
