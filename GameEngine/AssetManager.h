#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "vector2D.h"
#include "ECS.h"
#include "SDL_ttf.h"

class AssetManager
{
public:

	AssetManager(Manager* manager);
	~AssetManager();

	void CreateProjectile(Vector2D position, Vector2D velocity, int range, int speed, std::string textureId);
	void CreatePlayer(Vector2D position, int width, int height, float scale, std::string textureId);
	void CreateEnemy(Vector2D position, int width, int height, float scale, std::string textureId, std::string name);

	void AddTexture(std::string id, const char* filePath);
	SDL_Texture* GetTexture(std::string id);

	void AddFont(std::string id, std::string path, int fontSize);
	TTF_Font* GetFont(std::string id);

private:

	Manager* m_manager;
	std::map<std::string, SDL_Texture*> m_textures;
	std::map<std::string, TTF_Font*> m_fonts;
};