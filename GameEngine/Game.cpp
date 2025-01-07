#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "Map.h"

Manager g_manager;

SDL_Renderer* Game::m_renderer = nullptr;
SDL_Event Game::m_event;

SDL_Rect Game::m_camera = { 0, 0, 800, 640 };

std::vector<ColliderComponent*> Game::m_colliders;

bool Game::m_isRunning = false;

auto& skeleton(g_manager.addEntity());
auto& skeletonArcher(g_manager.addEntity());
auto& player(g_manager.addEntity());

const char* tileSet = "Assets/Maps/Dungeon_Tileset.png";

enum g_groupLabels : std::size_t 
{
	Enemies,
	Players,
	Maps,
	Colliders,
	Objects
};

auto& tiles(g_manager.getGroup(Maps));
auto& enemies(g_manager.getGroup(Enemies));
auto& players(g_manager.getGroup(Players));

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

	Map::LoadMap("Assets/map.map", 12, 10);

	/*skeleton.addComponent<TransformComponent>(100.0f, 100.0f, 3);
	skeleton.addComponent<SpriteComponent>("Assets/Skeleton/Idle.png");
	skeleton.addComponent<ColliderComponent>("Skeleton");
	skeleton.addGroup(Enemies);

	skeletonArcher.addComponent<TransformComponent>(300.0f, 150.0f, 3);
	skeletonArcher.addComponent<SpriteComponent>("Assets/Skeleton_Archer/Idle.png");
	skeletonArcher.addComponent<ColliderComponent>("Skeleton Archer");
	skeletonArcher.addGroup(Enemies);*/

	player.addComponent<TransformComponent>(2.2f);
	player.addComponent<SpriteComponent>("Assets/Shinobi/Shinobi_Animations.png", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player");
	player.addGroup(Players);

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

	for (auto collider : m_colliders)
	{
		Collision::AABB(player.getComponent<ColliderComponent>(), *collider);
	}	

	m_camera.x = player.getComponent<TransformComponent>().m_position.m_x - 400;
	m_camera.y = player.getComponent<TransformComponent>().m_position.m_y - 320;

	if (m_camera.x < 0)
		m_camera.x = 0;
	if (m_camera.y < 0)
		m_camera.y = 0;
	if (m_camera.x > m_camera.w)
		m_camera.x = m_camera.w;
	if (m_camera.y > m_camera.h)
		m_camera.y = m_camera.h;
}


void Game::render()
{
	SDL_RenderClear(m_renderer);
	for (auto& tile : tiles) 
	{
		tile->draw();
	}	
	for (auto& enemy : enemies)
	{
		enemy->draw();
	}
	for (auto& player : players)
	{
		player->draw();
	}
	SDL_RenderPresent(m_renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit;
	std::cout << "Game Cleaned\n";
}

void Game::AddTile(int srcX, int srcY, int xPos, int yPos)
{
	auto& tile(g_manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, tileSet);
	tile.addGroup(Maps);
}





