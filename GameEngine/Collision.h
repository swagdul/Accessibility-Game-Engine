#pragma once
#include "Game.h"

class ColliderComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect& rectA, const SDL_Rect& rectB);
	static bool AABB(const ColliderComponent& coliderA, const ColliderComponent& colliderB);
};

