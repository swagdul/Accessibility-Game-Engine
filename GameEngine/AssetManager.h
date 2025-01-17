#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "vector2D.h"
#include "ECS.h"

class AssetManager
{
public:

	AssetManager(Manager* manager);
	~AssetManager();

	void CreateProjectile(Vector2D position, Vector2D velocity, int range, int speed, std::string textureId);

	void AddTexture(std::string id, const char* filePath);
	SDL_Texture* GetTexture(std::string id);

private:

	Manager* m_manager;
	std::map<std::string, SDL_Texture*> m_textures;
};