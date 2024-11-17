#pragma once

#include <string>
#include "SDL.h"
#include "Components.h"

class ColliderComponent : public Component
{
public:

	SDL_Rect m_collider;
	std::string m_tag;

	TransformComponent* m_transform;

	ColliderComponent(std::string tag)
	{
		m_tag = tag;
	}

	void init() override
	{
		if (!m_entity->hasComponent<TransformComponent>())
		{
			m_entity->addComponent<TransformComponent>();
		}
		m_transform = &m_entity->getComponent<TransformComponent>();

		Game::m_colliders.push_back(this);
	}

	void update() override
	{
		m_collider.x = static_cast<int>(m_transform->m_position.m_x);
		m_collider.y = static_cast<int>(m_transform->m_position.m_y);
		m_collider.w = m_transform->m_width * m_transform->m_scale;
		m_collider.h = m_transform->m_height * m_transform->m_scale;
	}

};