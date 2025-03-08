#pragma once

#include "Components.h"
#include "SDL.h"
#include "Game.h"

class HealthComponent : public Component
{
public:

	HealthComponent(int health)
	{
		m_currentHealth = health;
		m_maxHealth = health;
	}

	HealthComponent(int health, bool isPlayer)
	{
		m_currentHealth = health;
		m_maxHealth = health;
		m_isPlayer = isPlayer;
	}

	~HealthComponent()
	{
	}

	void draw() override
	{

		if (m_isPlayer)
		{
			int windowWidth = 0, windowHeight = 0;
			SDL_GetRendererOutputSize(Game::m_renderer, &windowWidth, &windowHeight);

			m_barWidth = 300;
			m_barHeight = 30;
			m_marginX = 40;
			m_marginY = 190;

			SDL_Rect healthBarBg;
			healthBarBg.x = m_marginX;
			healthBarBg.y = windowWidth - m_marginY - m_barHeight;
			healthBarBg.w = m_barWidth;
			healthBarBg.h = m_barHeight;

			float healthPercentage = static_cast<float>(m_currentHealth / m_maxHealth);
			SDL_Rect healthBarFg = healthBarBg;
			healthBarFg.w = static_cast<int>(m_barWidth * healthPercentage);

			SDL_SetRenderDrawColor(Game::m_renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(Game::m_renderer, &healthBarBg);

			SDL_SetRenderDrawColor(Game::m_renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(Game::m_renderer, &healthBarFg);

			SDL_SetRenderDrawColor(Game::m_renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(Game::m_renderer, &healthBarBg);

		}
		else
		{
			auto& transform = m_entity->getComponent<TransformComponent>();

			m_barWidth = transform.m_width;
			m_barHeight = 5;
			m_barOffset = 20;

			SDL_Rect healthBarBg;
			healthBarBg.x = transform.m_position.m_x - Game::m_camera.x + (m_barWidth / 2);
			healthBarBg.y = transform.m_position.m_y - Game::m_camera.y - m_barOffset;
			healthBarBg.w = m_barWidth;
			healthBarBg.h = m_barHeight;

			float healthPercentage = static_cast<float>(m_currentHealth / m_maxHealth);
			SDL_Rect healthBarFg = healthBarBg;
			healthBarFg.w = static_cast<int>(m_barWidth * healthPercentage);

			SDL_SetRenderDrawColor(Game::m_renderer, 0, 255, 0, 255);
			SDL_RenderFillRect(Game::m_renderer, &healthBarBg);

			SDL_SetRenderDrawColor(Game::m_renderer, 255, 0, 0, 255);
			SDL_RenderFillRect(Game::m_renderer, &healthBarFg);

			SDL_SetRenderDrawColor(Game::m_renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(Game::m_renderer, &healthBarBg);
		}
	}

	int getCurrentHealth()
	{
		return m_currentHealth;
	}

	int getMaxHealth()
	{
		return m_maxHealth;
	}

	void setCurrentHealth(int health)
	{
		m_currentHealth = health;
	}

	void setMaxHealth(int health)
	{
		m_maxHealth = health;
	}

	void setIfPlayer(bool isPlayer)
	{
		m_isPlayer = isPlayer;
	}

	bool isPlayer()
	{
		return m_isPlayer;
	}

private:
	int m_currentHealth;
	int m_maxHealth;
	bool m_isPlayer;

	int m_barWidth;
	int m_barHeight;
	int m_barOffset;
	int m_marginX;
	int m_marginY;

};
