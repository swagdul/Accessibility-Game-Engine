#include "Game.h"
#include "TextureManager.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "Map.h"
#include "AssetManager.h"
#include "AccessibilityManager.h"
#include "MenuSystem.h"
#include "AccessibilityMenu.h"
#include "SaveLoad.h"
#include <sstream>

Map* g_map;
Manager g_manager;

SDL_Renderer* Game::m_renderer = nullptr;
SDL_Event Game::m_event;

SDL_Rect Game::m_camera = { 0, 0, 800, 640 };

AssetManager* Game::m_assets = new AssetManager(&g_manager);

bool Game::m_isRunning = false;

auto& skeleton(g_manager.addEntity());
auto& skeletonArcher(g_manager.addEntity());
auto& player(g_manager.addEntity());

auto& label(g_manager.addEntity());

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

	if (TTF_Init() == -1)
	{
		std::cout << "Error: SDL_TTF\n";
	}

	TTF_Font* font = TTF_OpenFont("Assets/arial.ttf", 24);
	MenuSystem menu(this, m_renderer, font, { "Start Game", "Options", "Exit" });

	int selectedOption = menu.DisplayMenu();

	if (selectedOption == -1 || selectedOption == 2) {
		m_isRunning = false;
		return;
	}

	TTF_CloseFont(font);

	m_assets->AddTexture("Terrain", "Assets/Maps/Dungeon_Tileset.png");
	m_assets->AddTexture("Player", "Assets/Shinobi/Shinobi_Animations.png");
	m_assets->AddTexture("Skeleton", "Assets/Skeleton/Idle.png");
	m_assets->AddTexture("Skeleton Archer", "Assets/Skeleton_Archer/Idle.png");
	m_assets->AddTexture("Projectile", "Assets/Projectile.png");
	m_assets->AddTexture("DefaultTexture", "Assets/Default/Idle.png");

	m_assets->AddFont("Arial", "Assets/Arial.ttf", AccessibilityManager::GetFontSize());

	//g_map = new Map("Terrain", 4, 16);

	g_map = new Map("Terrain", 2, 16);

	g_map->LoadMap("Assets/map.map", 25, 20);

	skeleton.setName("Skeleton");
	skeleton.addComponent<TransformComponent>(400.0f, 300.0f, 64, 128, 2);
	skeleton.addComponent<SpriteComponent>("Skeleton", true, 7, 100, true);
	skeleton.addComponent<ColliderComponent>("Skeleton");
	skeleton.addComponent<HealthComponent>(100, false);
	skeleton.addGroup(Enemies);

	skeletonArcher.setName("Skeleton Archer");
	skeletonArcher.addComponent<TransformComponent>(500.0f, 150.0f, 64, 128, 2);
	skeletonArcher.addComponent<SpriteComponent>("Skeleton Archer", true, 7, 100, true);
	skeletonArcher.addComponent<ColliderComponent>("Skeleton Archer");
	skeletonArcher.addComponent<HealthComponent>(100, false);
	skeletonArcher.addGroup(Enemies);

	player.setName("Player");
	player.addComponent<TransformComponent>(1.4f);
	player.addComponent<SpriteComponent>("Player", true);
	player.addComponent<KeyboardController>();
	player.addComponent<ColliderComponent>("Player");
	player.addComponent<HealthComponent>(100, true);
	player.addGroup(Players);


	SDL_Colour white = { 255, 255, 255, 255 };
	label.addComponent<UILabel>(10, 10, "Label", "Arial", white);
	label.getComponent<UILabel>().SetLabelText("Press F1 to open Accessibility Menu", "Arial");	//SDL_Colour white = { 255, 255, 255, 255 };


	m_assets->CreateProjectile(Vector2D(500, 500), Vector2D(2, 0), 200, 2, "Projectile");

	LoadEntities("entities.json");
}

auto& tiles(g_manager.getGroup(Game::Maps));
auto& colliders(g_manager.getGroup(Game::Colliders));
auto& enemies(g_manager.getGroup(Game::Enemies));
auto& players(g_manager.getGroup(Game::Players));
auto& projectiles(g_manager.getGroup(Game::Projectiles));

void Game::handleEvents()
{
	SDL_PollEvent(&m_event);

	if (m_event.type == SDL_KEYDOWN && m_event.key.keysym.sym == SDLK_F1) {
		AccessibilityMenu accessibilityMenu(this, m_renderer);
		accessibilityMenu.Run();
	}

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
	SDL_Rect playerCollider = player.getComponent<ColliderComponent>().m_collider;
	Vector2D playerPosition = player.getComponent<TransformComponent>().m_position;

	//std::stringstream stringStream;
	//stringStream << "Player position: " << playerPosition;
	//label.getComponent<UILabel>().SetLabelText(stringStream.str(), "Arial");

	g_manager.refresh();
	g_manager.update();

	for (auto& collider : colliders)
	{
		SDL_Rect boundaryCollider = collider->getComponent<ColliderComponent>().m_collider;

		if (Collision::AABB(boundaryCollider, playerCollider))
		{
			player.getComponent<TransformComponent>().m_position = playerPosition;
		}
	}

	for (auto& projectile : projectiles)
	{
		SDL_Rect projectileCollider = projectile->getComponent<ColliderComponent>().m_collider;

		if (Collision::AABB(projectileCollider, playerCollider))
		{
			projectile->destroy();
		}
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
	for (auto& collider : colliders)
	{
		collider->draw();
	}
	for (auto& enemy : enemies)
	{
		enemy->draw();
	}
	for (auto& player : players)
	{
		player->draw();
	}
	for (auto& projectile : projectiles)
	{
		projectile->draw();
	}
	label.draw();

	SDL_RenderPresent(m_renderer);
}

void Game::clean()
{
	std::vector<Entity*> entities = g_manager.getEntities();
	SaveEntities(entities, "entities.json");

	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
	std::cout << "Game Cleaned\n";
}


