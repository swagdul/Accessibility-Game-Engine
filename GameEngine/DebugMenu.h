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
	ModifyEntity,
	AdjustAppearance
};

class DebugMenu
{
public:

	DebugMenu(Game* game, SDL_Renderer* renderer);
	~DebugMenu();

	void AddLogMessage(const std::string& message);
	void ClearLogMessages();
	std::string TextInput();
	int GetIntInput();
	float GetFloatInput();
	void AddTextToMenu(std::string text);
	void Run();

private:
	Game* m_game;
	SDL_Renderer* m_renderer;
	bool m_isRunning;
	int m_selectedIndex;
	DebugMenuState m_state;

	int m_fontSize;
	SDL_Color m_normalColour;
	SDL_Color m_highlightColour;

	std::vector<std::string> m_menuOptions;
	std::vector<std::string> m_logMessages;

	void RenderMainMenu();
	void RenderSubMenu();
	void HandleEvent(SDL_Event& event);

	void CreateEntity();
	void ModifyEntity();
	void ListEntities();
	void AdjustAppearance();
	void DeleteEntity();
};

