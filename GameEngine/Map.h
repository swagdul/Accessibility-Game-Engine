#pragma once

#include <iostream>

class Map
{
public:

	Map(std::string textureId, int mapScale, int tileSize);
	~Map();

	void LoadMap(std::string filePath, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int xPos, int yPos);

private:
	std::string m_textureId;
	int m_mapScale;
	int m_tileSize;
	int m_scaledSize;
	
};

