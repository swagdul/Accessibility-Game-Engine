#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS.h"
#include "Components.h"

extern Manager g_manager;

Map::Map(const char* mapFilePath, int mapScale, int tileSize) : m_mapFilePath(mapFilePath), m_mapScale(mapScale), m_tileSize(tileSize)
{
	m_scaledSize = mapScale * tileSize;
}

Map::~Map()
{
}

void Map::LoadMap(std::string filePath, int sizeX, int sizeY)
{
	char tile;
	std::fstream map;
	map.open(filePath);

	int srcX;
	int srcY;
	
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			map.get(tile);
			srcY = atoi(&tile) * m_tileSize;
			map.get(tile);
			srcX = atoi(&tile) * m_tileSize;
			AddTile(srcX, srcY, x * m_scaledSize, y * m_scaledSize);
			map.ignore();
		}
	}

	map.ignore();

	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			map.get(tile);
			if (tile == '1')
			{
				auto& tileCollider(g_manager.addEntity());
				tileCollider.addComponent<ColliderComponent>("terrain", x * m_scaledSize, y * m_scaledSize, m_scaledSize);
				tileCollider.addGroup(Game::Colliders);
			}
			map.ignore();
		}
	}



	map.close();
}

void Map::AddTile(int srcX, int srcY, int xPos, int yPos)
{
	auto& tile(g_manager.addEntity());
	tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, m_tileSize, m_mapScale, m_mapFilePath);
	tile.addGroup(Game::Maps);
}


