#pragma once

#include <iostream>
#include <vector>
#include <tuple>

class Map
{
public:

	Map();
	~Map();

	static void LoadMap(std::string filePath, int sizeX, int sizeY);
	
};

