#include "Collision.h"

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
