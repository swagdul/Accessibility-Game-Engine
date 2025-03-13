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

	//std::map<const char*, Animation> m_animations;
	std::map<std::string, Animation> m_animations;

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
	
	SpriteComponent(std::string textureId, bool isAnimated, int frames, int speed, bool isFlipped)
	{
		m_isAnimated = isAnimated;
		m_isFlipped = isFlipped;
		m_speed = speed;
		m_frames = frames;

		Animation idle = Animation(0, frames, speed);
		Animation walk = Animation(1, frames, speed);

		m_animations.emplace("Idle", idle);
		m_animations.emplace("Walk", walk);
		
		PlayAnimation("Idle");

		setTexture(textureId);

		m_spriteFlip = isFlipped ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	}

	~SpriteComponent()
	{

	}
	
	void setTexture(std::string textureId)
	{
		m_texture = Game::m_assets->GetTexture(textureId);
		m_textureID = textureId;
	}

	std::string getTextureID()
	{
		return m_textureID;
	}

	void enableAnimation()
	{
		m_isAnimated = true;
	}

	bool isAnimated()
	{
		return m_isAnimated;
	}

	void init() override
	{
		m_transform = &m_entity->getComponent<TransformComponent>();

		if (m_srcRect.w == 0 || m_srcRect.h == 0)
		{
			m_srcRect = { 0, 0, m_transform->m_width, m_transform->m_height };
		}

		if (m_destRect.w == 0 || m_destRect.h == 0)
		{
			m_destRect = { 64, 64,
						   static_cast<int>(m_transform->m_width * m_transform->m_scale),
						   static_cast<int>(m_transform->m_height * m_transform->m_scale) };
		}
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
		//m_frames = m_animations[animationName].m_frames;
		//m_animationIndex = m_animations[animationName].m_index;
		//m_speed = m_animations[animationName].m_speed;

		auto it = m_animations.find(animationName);
		if (it != m_animations.end())
		{
			m_frames = it->second.m_frames;
			m_animationIndex = it->second.m_index;
			m_speed = it->second.m_speed;
		}
	}

	int getAnimatiomFrames()
	{
		return m_frames;
	}

	void setAnimationFrames(int frames)
	{
		m_frames = frames;
	}

	int getAnimationSpeed()
	{
		return m_speed;
	}

	void setAnimationSpeed(int speed)
	{
		m_speed = speed;
	}

	bool isFlipped()
	{
		return m_isFlipped;
	}

	void setFlip(bool flip)
	{
		m_isFlipped = flip;
	}

private:
	TransformComponent* m_transform;
	SDL_Texture* m_texture;
	SDL_Rect m_srcRect, m_destRect;

	bool m_isAnimated = false;
	int m_frames = 0;
	int m_speed = 100;
	bool m_isFlipped;

	std::string m_textureID;

};