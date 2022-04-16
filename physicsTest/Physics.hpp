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
    static void ResolveCollisions(std::vector<Unit*> units);
    
private:
    static bool CreateManifold(Manifold* m);
    static void ResolveCollision(Unit* a, Unit* b);
};

#endif /* Physics_hpp */
