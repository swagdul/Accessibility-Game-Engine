#pragma once

#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"

class SpriteComponent : public Component
{
public:

	SpriteComponent() = default;
	SpriteComponent(const char* filePath)
	{
		setTexture(filePath);
	}
	
	void setTexture(const char* filePath)
	{
		m_texture = TextureManager::LoadTexture(filePath);
	}

	void init() override
	{
		m_position = &m_entity->getComponent<PositionComponent>();

		m_srcRect = { 0, 0, 128, 128 };
		m_destRect = { 64, 64, 128, 128 };
	}

	void update() override
	{
		m_destRect.x = m_position->getxPos();
		m_destRect.y = m_position->getyPos();
	}

	void draw() override
	{
		TextureManager::Draw(m_texture, m_srcRect, m_destRect);
	}

private:
	PositionComponent* m_position;
	SDL_Texture* m_texture;
	SDL_Rect m_srcRect, m_destRect;

};