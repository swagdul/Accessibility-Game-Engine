#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	void Init(const char* title, int xPos, int yPos, int width, int height, bool isFullscreen);
	
	void HandleEvents();
	void Update();
	bool IsRunning() { return m_isRunning; }
	void Render();
	void Clean();

	static SDL_Renderer* m_renderer;

private:
	bool m_isRunning = false;
	SDL_Window* m_window;
};

