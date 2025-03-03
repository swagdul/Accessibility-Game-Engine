#pragma once
#include <vector>
#include <string>
#include "SDL.h"
#include "Game.h"
#include "ScreenReader.h"

class DebugMenu
{
public:

	DebugMenu(Game* game, SDL_Renderer* renderer);
	~DebugMenu();

	void Run();

private:
	Game* m_game;
	SDL_Renderer* m_renderer;
	bool m_isRunning;
	int m_selectedIndex;

	std::vector<std::string> m_menuOptions;

	void Render();
	void HandleEvent(SDL_Event& event);

	void CreateEntity();
	void ModifyEntity();
	void ListEntities();
	void AddComponentToEntity();
};

