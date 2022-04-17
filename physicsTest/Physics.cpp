#include "Physics.hpp"
#include "Unit.hpp"

bool Physics::CreateManifold(Manifold* m)
{
    Unit* A = m->A;
    Unit* B = m->B;
    Vec normal = B->pos - A->pos;
    int radius = A->radius + B->radius;
 
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
        m->penetration = A->radius;
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
        Vec rVelocity = b->velocity - a->velocity;
        
        int velAlongNormal = rVelocity.Dot(m.normal) / Vec::SCALE;

        if (velAlongNormal > 0) return; // if bodies are separating, don't resolve
        
        int inverseMassSum = (a->mass + b->mass);
        int j;

        if (inverseMassSum != 0)
        {
            j = -Vec::SCALE * velAlongNormal / inverseMassSum + m.penetration;
            Vec impulse = (m.normal * j / Vec::SCALE);
            a->velocity -= (impulse * a->mass) / Vec::SCALE;
            b->velocity += (impulse * b->mass) / Vec::SCALE;
        }
    }
}

void Physics::ResolveCollisions(std::vector<Unit*>* units)
{
    int processedUntil = 0;
    
    for (auto unit : *units)
    {
        for (auto otherUnit : *units)
        {
            if (unit != otherUnit && otherUnit->orderNumber > processedUntil)
            {
                ResolveCollision(unit, otherUnit);
            }
        }
        processedUntil = unit->orderNumber;
    }
}
