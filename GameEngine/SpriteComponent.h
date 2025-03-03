#pragma once

#include "Components.h"
#include "SDL.h"
#include "TextureManager.h"
#include "Animation.h"
#include <map>
#include "AssetManager.h"

class SpriteComponent : public Component
{
public:

	int m_animationIndex = 0;

	std::map<const char*, Animation> m_animations;

	SDL_RendererFlip m_spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string textureId)
	{
		setTexture(textureId);
	}

	SpriteComponent(std::string textureId, bool isAnimated)
	{
		m_isAnimated = isAnimated;

		Animation idle = Animation(0, 6, 100);
		Animation walk = Animation(1, 8, 100);

		m_animations.emplace("Idle", idle);
		m_animations.emplace("Walk", walk);
		
		PlayAnimation("Idle");

		setTexture(textureId);
	}	
	
	SpriteComponent(std::string textureId, bool isAnimated, int frames, int speed)
	{
		m_isAnimated = isAnimated;

		Animation idle = Animation(0, frames, speed);
		Animation walk = Animation(1, frames, speed);

		m_animations.emplace("Idle", idle);
		m_animations.emplace("Walk", walk);
		
		PlayAnimation("Idle");

		setTexture(textureId);
	}

	~SpriteComponent()
	{

	}
	
	void setTexture(std::string textureId)
	{
		m_texture = Game::m_assets->GetTexture(textureId);
	}

	void enableAnimation()
	{
		m_isAnimated = true;
	}

	void init() override
	{
		m_transform = &m_entity->getComponent<TransformComponent>();

		m_srcRect = { 0, 0, m_transform->m_width, m_transform->m_height };
		m_destRect = { 64, 64, 128, 128 };
	}

	void update() override
	{
		if (m_isAnimated)
		{
			m_srcRect.x = m_srcRect.w * static_cast<int>((SDL_GetTicks() / m_speed) % m_frames);
		}

		m_srcRect.y = m_animationIndex * m_transform->m_height;

		m_destRect.x = static_cast<int>(m_transform->m_position.m_x) - Game::m_camera.x;
		m_destRect.y = static_cast<int>(m_transform->m_position.m_y) - Game::m_camera.y;
		m_destRect.w = static_cast<int>(m_transform->m_width * m_transform->m_scale);
		m_destRect.h = static_cast<int>(m_transform->m_height * m_transform->m_scale);
	}

	void draw() override
	{
		TextureManager::Draw(m_texture, m_srcRect, m_destRect, m_spriteFlip);
	}

	void PlayAnimation(const char* animationName)
	{
		m_frames = m_animations[animationName].m_frames;
		m_animationIndex = m_animations[animationName].m_index;
		m_speed = m_animations[animationName].m_speed;
	}

private:
	TransformComponent* m_transform;
	SDL_Texture* m_texture;
	SDL_Rect m_srcRect, m_destRect;

	bool m_isAnimated = false;
	int m_frames = 0;
	int m_speed = 100;

};