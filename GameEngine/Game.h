#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Game
{
public:

	Game();
	~Game();

	void init(const char* title, int xPos, int yPos, int width, int height, bool isFullscreen);
	
	void handleEvents();
	void update();
	bool isRunning() { return m_isRunning; }
	void render();
	void clean();

private:

	bool m_isRunning = false;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};

