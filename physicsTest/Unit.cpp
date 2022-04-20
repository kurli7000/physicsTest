#include "Unit.hpp"

Unit::Unit(const Unit &u) :
    pos(u.pos),
    radius(u.radius),
    velocity(u.velocity),
    mass(u.mass),
    orderNumber(u.orderNumber)
{
}

Unit& Unit::operator = (const Unit& other)
{
    pos = other.pos;
    velocity = other.velocity;
    radius = other.radius;
    mass = other.mass;
    orderNumber = other.orderNumber;
    return *this;
}

void Unit::IncreaseVelocity(Vec velocityIncrease)
{
    velocity += velocityIncrease;
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
            u1->mass = 10;
            u1->radius = RADIUS * 3;
        }
        else
        {
            u1->mass = 1;
            u1->radius = RADIUS / 2 + (rand() % 400);
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

void Unit::ReplaceData(std::vector<Unit*>* src, std::vector<Unit*>* dst)
{
    assert((*src).size() == (*dst).size());
    
    for (int i = 0; i < (*src).size(); i++)
    {
        Unit& srcunit = *(*src)[i];
        Unit& dstunit = *(*dst)[i];
        dstunit = srcunit;
    }
}
