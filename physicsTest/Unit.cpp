#include "Unit.hpp"

void Unit::Precalc(std::vector<Unit*>* units, int numUnits)
{
    for (int i = 0; i < numUnits; i++)
    {
        Unit* u1 = new Unit();
        u1->m_Pos = Vec((rand() % 32) * Vec::SCALE, (rand() % 32) * Vec::SCALE);
        u1->m_Velocity = Vec((rand() % 32) - 16, (rand() % 32) - 16);
        if (i == 0)
        {
            u1->m_Mass = 1;
            u1->m_Radius = RADIUS * 3;
        }
        else
        {
            u1->m_Mass = 50;
            u1->m_Radius = RADIUS;
        }
        u1->m_OrderNumber = i + 1;
        units->push_back(u1);
    }
}

void Unit::CopyUnits(std::vector<Unit*>* src, std::vector<Unit*>* dst)
{
    for (auto u1 : *src)
    {
        Unit* u2 = new Unit();
        u2->m_Pos = u1->m_Pos;
        u2->m_Radius = u1->m_Radius;
        u2->m_Velocity = u1->m_Velocity;
        u2->m_Mass = u1->m_Mass;
        u2->m_OrderNumber = u1->m_OrderNumber;
        dst->push_back(u2);
    }
}

