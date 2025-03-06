#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* filePath, int xPos, int yPos)
{
	m_objectTexture = TextureManager::LoadTexture(filePath);
	m_xPos = xPos;
	m_yPos = yPos;
}

void GameObject::Update()
{
	m_xPos++;
	m_yPos++;

	m_srcRect = { 0, 0, 128, 128 };
	m_destRect = { m_xPos, m_yPos, m_srcRect.w * 2, m_srcRect.h * 2 };
}

void GameObject::Render()
{
	SDL_RenderCopy(Game::m_renderer, m_objectTexture, &m_srcRect, &m_destRect);
}
