#include "Collision.h"
#include "ColliderComponent.h"

bool Collision::AABB(const SDL_Rect& rectA, const SDL_Rect& rectB)
{
    if (
        rectA.x + rectA.w >= rectB.x &&
        rectA.x + rectA.w >= rectA.x &&
        rectA.y + rectA.h >= rectB.y &&
        rectA.y + rectA.h >= rectA.y
        )
    {
        return true;
    }

    return false;
}

bool Collision::AABB(const ColliderComponent& colliderA, const ColliderComponent& colliderB)
{

    if (AABB(colliderA.m_collider, colliderB.m_collider))
    {
        std::cout << colliderA.m_tag << " hit: " << colliderB.m_tag << std::endl;

        return true;
    }
    else 
    {
        return false;
    }
}
