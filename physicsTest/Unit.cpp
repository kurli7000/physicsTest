#include "Unit.hpp"

Unit::Unit(const Unit &u) :
    pos(u.pos),
    radius(u.radius),
    velocity(u.velocity),
    mass(u.mass),
    orderNumber(u.orderNumber)
{
}

void Unit::Precalc(std::vector<Unit*>* units, int numUnits)
{
    for (int i = 0; i < numUnits; i++)
    {
        Unit* u1 = new Unit();
        u1->pos = Vec((rand() % 32) * Vec::SCALE, (rand() % 32) * Vec::SCALE);
        u1->velocity = Vec((rand() % 32) - 16, (rand() % 32) - 16);
        if (i == 0)
        {
            u1->mass = 1;
            u1->radius = RADIUS * 3;
        }
        else
        {
            u1->mass = 50;
            u1->radius = RADIUS;
        }
        u1->orderNumber = i + 1;
        units->push_back(u1);
    }
}

void Unit::CopyUnits(std::vector<Unit*>* src, std::vector<Unit*>* dst)
{
    for (auto u1 : *src)
    {
        Unit* u2 = new Unit(*u1);
        dst->push_back(u2);
    }
}
