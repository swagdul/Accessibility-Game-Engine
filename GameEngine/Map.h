#pragma once

#include <iostream>

class Map
{
public:

	Map();
	~Map();

	static void LoadMap(std::string filePath, int sizeX, int sizeY);
};

