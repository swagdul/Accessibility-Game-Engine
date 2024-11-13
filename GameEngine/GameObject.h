#pragma once
#include "Game.h"

class GameObject
{
public:

	GameObject(const char* textureSheet, SDL_Renderer* renderer, int x, int y);
	~GameObject();

	void Update();
	void Render();

private:

	int m_xPos;
	int m_yPos;

	SDL_Texture* m_objectTexture;
	SDL_Rect m_srcRect;
	SDL_Rect m_destRect;
	SDL_Renderer* m_renderer;

};

