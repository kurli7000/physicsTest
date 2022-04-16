#include "Physics.hpp"
#include "Unit.hpp"

bool Physics::CreateManifold(Manifold* m)
{
    Unit* A = m->A;
    Unit* B = m->B;
    Vec normal = B->m_Pos - A->m_Pos;
    int radius = A->m_Radius + B->m_Radius;
 
    if (normal.GetLengthSquared() > radius * radius) return false;
    
    int distance = normal.GetLength();
 
    if (distance != 0)
    {
        m->penetration = (radius - distance);
        m->normal = normal * Vec::SCALE / distance;
        return true;
    }
    else
    {
        m->penetration = A->m_Radius;
        m->normal = Vec(Vec::SCALE, 0);
        return true;
    }
}

void Physics::ResolveCollision(Unit* a, Unit* b)
{
    Manifold m(a, b);
        
    // Adapted from https://gamedevelopment.tutsplus.com/tutorials/how-to-create-a-custom-2d-physics-engine-the-basics-and-impulse-resolution--gamedev-6331
    if (CreateManifold(&m))
    {
        // relative velocity
        Vec rVelocity = b->m_Velocity - a->m_Velocity;
        
        int velAlongNormal = rVelocity.Dot(m.normal) / Vec::SCALE;

        if (velAlongNormal > 0) return; // if bodies are separating, don't resolve
        
        int inverseMassSum = (a->m_Mass + b->m_Mass);
        int j;

        if (inverseMassSum != 0)
        {
            j = -Vec::SCALE * velAlongNormal / inverseMassSum + m.penetration;
            Vec impulse = (m.normal * j / Vec::SCALE);
            a->m_Velocity -= (impulse * a->m_Mass) / Vec::SCALE;
            b->m_Velocity += (impulse * b->m_Mass) / Vec::SCALE;
        }
    }
}

void Physics::ResolveCollisions(std::vector<Unit*> units)
{
    int processedUntil = 0;
    
    for (auto unit : units)
    {
        for (auto otherUnit : units)
        {
            if (unit != otherUnit && otherUnit->m_OrderNumber > processedUntil)
            {
                ResolveCollision(unit, otherUnit);
            }
        }
        processedUntil = unit->m_OrderNumber;
    }
}
