#pragma once

#include <string>
#include "SDL.h"
#include "Components.h"
#include "TextureManager.h"

class ColliderComponent : public Component
{
public:

	SDL_Rect m_collider;
	std::string m_tag;

	SDL_Texture* m_texture;
	SDL_Rect m_srcRect, m_destRect;

	TransformComponent* m_transform;

	ColliderComponent(std::string tag)
	{
		m_tag = tag;
	}

	ColliderComponent(std::string tag, int xPos, int yPos, int size)
	{
		m_tag = tag;
		m_collider.x = xPos;
		m_collider.y = yPos;
		m_collider.h = m_collider.w = size;
	}

	void init() override
	{
		if (!m_entity->hasComponent<TransformComponent>())
		{
			m_entity->addComponent<TransformComponent>();
		}
		m_transform = &m_entity->getComponent<TransformComponent>();

		m_texture = TextureManager::LoadTexture("Assets/collision_texture.png");
		m_srcRect = { 0, 0, 64, 64 };
		m_destRect = { m_collider.x, m_collider.y, m_collider.w, m_collider.h };

	}

	void update() override
	{
		if (m_tag != "terrain")
		{
			m_collider.x = static_cast<int>(m_transform->m_position.m_x);
			m_collider.y = static_cast<int>(m_transform->m_position.m_y);
			m_collider.w = (m_transform->m_width / 2) * m_transform->m_scale;
			m_collider.h = (m_transform->m_height / 2) * m_transform->m_scale;
		}

		m_destRect.x = m_collider.x - Game::m_camera.x;
		m_destRect.y = m_collider.y - Game::m_camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(m_texture, m_srcRect, m_destRect, SDL_FLIP_NONE);
	}

};