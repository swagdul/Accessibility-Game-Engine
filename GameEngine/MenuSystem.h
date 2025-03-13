#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include "Game.h"
#include <vector>
#include <string>

class MenuSystem
{
public:
	MenuSystem(Game* game, SDL_Renderer* renderer, TTF_Font* font, const std::vector<std::string>& options);
	~MenuSystem();

	int DisplayMenu();

private:
	Game* m_game;
	SDL_Renderer* m_renderer;
	TTF_Font* m_font;
	std::vector<std::string> m_menuLabels;
	std::vector<SDL_Surface*> m_menuSurfaces;
	std::vector<SDL_Texture*> m_menuTextures;
	std::vector<SDL_Rect> m_menuPositions;
	std::vector<bool> m_selected;
	SDL_Color m_menuColours[2] = { {255, 255, 255}, {255, 0, 0} };
	int m_selectedIndex;

	void RenderMenu();
	void Cleanup();
};

