#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class ColliderComponent;

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

	static SDL_Renderer* m_renderer;
	static SDL_Event m_event;
	static bool m_isRunning;
	static SDL_Rect m_camera;

	enum g_groupLabels : std::size_t
	{
		Enemies,
		Players,
		Maps,
		Colliders,
		Objects
	};

private:
	SDL_Window* m_window;
};

