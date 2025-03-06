#pragma once
#include <vector>
#include <string>
#include "SDL.h"
#include "Game.h"
#include "ScreenReader.h"

enum class DebugMenuState
{
	MainMenu,
	ListEntities,
	CreateEntity,
	ModifyEntity
};

class DebugMenu
{
public:

	DebugMenu(Game* game, SDL_Renderer* renderer);
	~DebugMenu();

	void AddLogMessage(const std::string& message);
	void Run();

private:
	Game* m_game;
	SDL_Renderer* m_renderer;
	bool m_isRunning;
	int m_selectedIndex;

	std::vector<std::string> m_menuOptions;
	std::vector<std::string> m_logMessages;

	void Render();
	void RenderSubMenu(const std::string& title);
	void HandleEvent(SDL_Event& event);

	void CreateEntity();
	void ModifyEntity();
	void ListEntities();

};

