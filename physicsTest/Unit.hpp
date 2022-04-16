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

    static void Precalc(std::vector<Unit*>* units, int numUnits);
    static void CopyUnits(std::vector<Unit*>* src, std::vector<Unit*>* dst);
    
    Vec pos;
    Vec velocity;
    int radius;
    int mass;
    int orderNumber;
    
private:
    static const int RADIUS = 600;
};

#endif /* Unit_hpp */
