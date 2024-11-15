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

	~SpriteComponent()
	{
		SDL_DestroyTexture(m_texture);
	}
	
	void setTexture(const char* filePath)
	{
		m_texture = TextureManager::LoadTexture(filePath);
	}

	void init() override
	{
		m_transform = &m_entity->getComponent<TransformComponent>();

		m_srcRect = { 0, 0, m_transform->m_width, m_transform->m_height };
		m_destRect = { 64, 64, 128, 128 };
	}

	void update() override
	{
		m_destRect.x = (int)m_transform->m_position.m_x;
		m_destRect.y = (int)m_transform->m_position.m_y;
		m_destRect.w = m_transform->m_width * m_transform->m_scale;
		m_destRect.h = m_transform->m_height * m_transform->m_scale;
		
	}

	void draw() override
	{
		TextureManager::Draw(m_texture, m_srcRect, m_destRect);
	}

private:
	TransformComponent* m_transform;
	SDL_Texture* m_texture;
	SDL_Rect m_srcRect, m_destRect;

};