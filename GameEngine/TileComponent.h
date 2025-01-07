#pragma once

#include "ECS.h"
#include "SDL.h"

class TileComponent : public Component
{
public:

	SDL_Texture* m_texture;
	SDL_Rect m_srcRect, m_destRect;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(m_texture);
	}

	TileComponent(int srcX, int srcY, int xPos, int yPos, const char* filePath)
	{
		m_texture = TextureManager::LoadTexture(filePath);

		m_srcRect.x = srcX;
		m_srcRect.y = srcY;
		m_srcRect.w = 64;
		m_srcRect.h = 64;

		m_destRect.x = xPos;
		m_destRect.y = yPos;
		m_destRect.w = 128;
		m_destRect.h = 128;
	}

	void draw() override
	{
		TextureManager::Draw(m_texture, m_srcRect, m_destRect, SDL_FLIP_NONE);
	}
};