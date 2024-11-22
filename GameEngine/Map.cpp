#include "Map.h"
#include "Game.h"
#include <fstream>

Map::Map()
{
}

Map::~Map()
{
}

void Map::LoadMap(std::string filePath, int sizeX, int sizeY)
{
	char tile;
	std::fstream map;
	map.open(filePath);
	
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			map.get(tile);
			Game::AddTile(atoi(&tile), x * 32, y * 32);
			map.ignore();
		}
	}

	map.close();
}
