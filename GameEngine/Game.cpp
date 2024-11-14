#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"

GameObject* skeleton;
GameObject* skeletonArcher;

SDL_Renderer* Game::m_renderer = nullptr;

Game::Game() 
{
}

Game::~Game()
{
}

void Game::Init(const char* title, int xPos, int yPos, int width, int height, bool isFullscreen)
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

	skeleton = new GameObject("Assets/Skeleton/Idle.png", 0, 0);
	skeletonArcher = new GameObject("Assets/Skeleton_Archer/Idle.png", 100, 100);
}

void Game::HandleEvents()
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

void Game::Update()
{
	skeleton->Update();
	skeletonArcher->Update();
}

void Game::Render()
{
	SDL_RenderClear(m_renderer);
	skeleton->Render();
	skeletonArcher->Render();
	SDL_RenderPresent(m_renderer);
}

void Game::Clean()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit;
	std::cout << "Game Cleaned\n";
}

