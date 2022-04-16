#ifndef Unit_hpp
#define Unit_hpp

#include <vector>
#include "Vec.h"

class Unit
{
public:
    Unit() {};
    ~Unit() {};
    static void Precalc(std::vector<Unit*>* units, int numUnits);
    static void CopyUnits(std::vector<Unit*>* src, std::vector<Unit*>* dst);
    
    Vec m_Pos;
    Vec m_Velocity;
    int m_Radius;
    int m_Mass;
    int m_OrderNumber;
    
private:
    static const int RADIUS = 600;
};

#endif /* Unit_hpp */
