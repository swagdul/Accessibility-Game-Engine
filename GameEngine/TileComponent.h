#pragma once

#include "ECS.h"
#include "SDL.h"

class TileComponent : public Component
{
public:

	SDL_Texture* m_texture;
	SDL_Rect m_srcRect, m_destRect;
	Vector2D m_postion;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(m_texture);
	}

	TileComponent(int srcX, int srcY, int xPos, int yPos, const char* filePath)
	{
		m_texture = TextureManager::LoadTexture(filePath);

		m_postion.m_x = xPos;
		m_postion.m_y = yPos;

		m_srcRect.x = srcX;
		m_srcRect.y = srcY;
		m_srcRect.w = 64;
		m_srcRect.h = 64;

		m_destRect.x = xPos;
		m_destRect.y = yPos;
		m_destRect.w = 128;
		m_destRect.h = 128;
	}

	void update() override
	{
		m_destRect.x = m_postion.m_x - Game::m_camera.x;
		m_destRect.y = m_postion.m_y - Game::m_camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(m_texture, m_srcRect, m_destRect, SDL_FLIP_NONE);
	}
};