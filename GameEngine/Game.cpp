#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"

Manager g_manager;

SDL_Renderer* Game::m_renderer = nullptr;
SDL_Event Game::m_event;

auto& skeleton(g_manager.addEntity());
auto& skeletonArcher(g_manager.addEntity());

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

	skeleton.addComponent<TransformComponent>(2);
	skeleton.addComponent<SpriteComponent>("Assets/Skeleton/Idle.png");
	skeleton.addComponent<KeyboardController>();
	skeleton.addComponent<ColliderComponent>("Skeleton");

	skeletonArcher.addComponent<TransformComponent>(300.0f, 300.0f, 2);
	skeletonArcher.addComponent<SpriteComponent>("Assets/Skeleton_Archer/Idle.png");
	skeletonArcher.addComponent<ColliderComponent>("Skeleton Archer");
}

void Game::handleEvents()
{
	SDL_PollEvent(&m_event);

	switch (m_event.type)
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
	g_manager.refresh();
	g_manager.update();

	if (Collision::AABB(skeleton.getComponent<ColliderComponent>().m_collider, 
		skeletonArcher.getComponent<ColliderComponent>().m_collider))
	{
		skeleton.getComponent<TransformComponent>().m_velocity * -1;
		std::cout << "Collision!\n";
	}
}

void Game::render()
{
	SDL_RenderClear(m_renderer);
 	g_manager.draw();
	SDL_RenderPresent(m_renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit;
	std::cout << "Game Cleaned\n";
}

