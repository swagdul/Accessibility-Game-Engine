#pragma once

#include <iostream>

class Map
{
public:

	Map(const char* mapFilePath, int mapScale, int tileSize);
	~Map();

	void LoadMap(std::string filePath, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int xPos, int yPos);

private:
	const char* m_mapFilePath;
	int m_mapScale;
	int m_tileSize;
	int m_scaledSize;
	
};

