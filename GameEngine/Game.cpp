#include "Game.h"
#include "TextureManager.h"

SDL_Texture* skeletonTex;
SDL_Rect srcRect;
SDL_Rect destRect;

Game::Game() 
{
}

Game::~Game()
{
}

void Game::init(const char* title, int xPos, int yPos, int width, int height, bool isFullscreen)
{
	int flags = 0;

	if (isFullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialiased!...\n";

		m_window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);

		if (m_window)
		{
			std::cout << "Window created!\n";
		}

		m_renderer = SDL_CreateRenderer(m_window, -1, 0);

		if (m_renderer)
		{
			SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
			std::cout << "Renderer created!\n";
		}

		m_isRunning = true;
	}
	else
	{
		m_isRunning = false;
	}

	skeletonTex = TextureManager::LoadTexture("Assets/Idle.png", m_renderer);
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		m_isRunning = false;
		break;

	default:
		break;
	}
}

void Game::update()
{
	destRect.h = 128;
	destRect.w = 128;

	srcRect.h = 128;
	srcRect.w = 128;
}

void Game::render()
{
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, skeletonTex, &srcRect, &destRect);
	SDL_RenderPresent(m_renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit;
	std::cout << "Game Cleaned\n";
}

