#pragma once

#include "ECS.h"
#include "AssetManager.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class UILabel : public Component
{
public:

	UILabel(int xPos, int yPos, std::string text, std::string font, SDL_Color& colour) : m_labelText(text), m_labelFont(font), m_textColour(colour)
	{
		m_position.x = xPos;
		m_position.y = yPos;

		SetLabelText(m_labelText, m_labelFont);
	}

	~UILabel()
	{

	}

	void SetLabelText(std::string text, std::string font)
	{
		SDL_Surface* surface = TTF_RenderText_Blended(Game::m_assets->GetFont(font), text.c_str(), m_textColour);
		m_labelTexture = SDL_CreateTextureFromSurface(Game::m_renderer, surface);
		SDL_FreeSurface(surface);

		SDL_QueryTexture(m_labelTexture, nullptr, nullptr, &m_position.w, &m_position.h);
	}

	void draw() override
	{
		SDL_RenderCopy(Game::m_renderer, m_labelTexture, nullptr, &m_position);
	}

private:

	SDL_Rect m_position;
	std::string m_labelText;
	std::string m_labelFont;
	SDL_Color m_textColour;
	SDL_Texture* m_labelTexture;
};
