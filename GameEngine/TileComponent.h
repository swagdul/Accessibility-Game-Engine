#pragma once

#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SDL.h"

class TileComponent : public Component
{
public:
	TransformComponent* m_transform;
	SpriteComponent* m_sprite;

	SDL_Rect m_tileRect;
	int m_tileID;
	const char* m_path;

	TileComponent() = default;

	TileComponent(int xPos, int yPos, int width, int height, int id)
	{
		m_tileRect.x = xPos;
		m_tileRect.y = yPos;
		m_tileRect.w = width;
		m_tileRect.h = height;
		m_tileID = id;

		switch (m_tileID)
		{
		case 0:
			m_path = "Craftpix TileSet/PNG/Ground_rocks.png";
			break;
		default:
			break;
		}
		
	}

	void init() override
	{
		m_entity->addComponent<TransformComponent>(m_tileRect.x, m_tileRect.y, m_tileRect.w, m_tileRect.h, 1);
		m_transform = &m_entity->getComponent<TransformComponent>();

		m_entity->addComponent<SpriteComponent>();
		m_sprite = &m_entity->getComponent<SpriteComponent>();
	}

};