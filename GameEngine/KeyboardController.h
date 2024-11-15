#pragma once

#include "Game.h"
#include "Components.h"
#include <unordered_map>

class KeyboardController : public Component
{
public:
	TransformComponent* m_transform;
	const Uint8* m_keystates = SDL_GetKeyboardState(NULL);

	void init() override
	{
		m_transform = &m_entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		m_transform->m_velocity.m_x = 0;
		m_transform->m_velocity.m_y = 0;

		if (m_keystates[SDL_SCANCODE_W]) m_transform->m_velocity.m_y =  - 1;
		if (m_keystates[SDL_SCANCODE_A]) m_transform->m_velocity.m_x =  - 1;
		if (m_keystates[SDL_SCANCODE_S]) m_transform->m_velocity.m_y = 1;
		if (m_keystates[SDL_SCANCODE_D]) m_transform->m_velocity.m_x = 1;
	}
};