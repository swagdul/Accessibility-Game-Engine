#pragma once

#include "ECS.h"
#include "SDL.h"

class TileComponent : public Component
{
public:

	SDL_Texture* m_texture;
	SDL_Rect m_srcRect, m_destRect;
	Vector2D m_position;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(m_texture);
	}

	TileComponent(int srcX, int srcY, int xPos, int yPos, int tileSize, int tileScale, std::string textureId)
	{
		m_texture = Game::m_assets->GetTexture(textureId);

		m_position.m_x = static_cast<int>(xPos);
		m_position.m_y = static_cast<int>(yPos);

		m_srcRect.x = srcX;
		m_srcRect.y = srcY;
		m_srcRect.w = tileSize;
		m_srcRect.h = tileSize;

		m_destRect.x = xPos;
		m_destRect.y = yPos;
		m_destRect.w = tileSize * tileScale;
		m_destRect.h = tileSize * tileScale;
	}

	void update() override
	{
		m_destRect.x = m_position.m_x - Game::m_camera.x;
		m_destRect.y = m_position.m_y - Game::m_camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(m_texture, m_srcRect, m_destRect, SDL_FLIP_NONE);
	}
};