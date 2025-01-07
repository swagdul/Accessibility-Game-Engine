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

	int srcX;
	int srcY;
	
	for (int y = 0; y < sizeY; y++)
	{
		for (int x = 0; x < sizeX; x++)
		{
			map.get(tile);
			srcY = atoi(&tile) * 64;
			map.get(tile);
			srcX = atoi(&tile) * 64;
			Game::AddTile(srcX, srcY, x * 128, y * 128);
			map.ignore();
		}
	}

	map.close();
}


