#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* textureSheet, SDL_Renderer* renderer, int xPos, int yPos)
{
	m_renderer = renderer;
	m_objectTexture = TextureManager::LoadTexture(textureSheet, renderer);

	m_xPos = xPos;
	m_yPos = yPos;

}

void GameObject::Update()
{
	m_xPos++;
	m_yPos++;

	m_srcRect.h = 128;
	m_srcRect.w = 128;
	m_srcRect.x = 0;
	m_srcRect.y = 0;

	m_destRect.x = m_xPos;
	m_destRect.y = m_xPos;
	m_destRect.w = m_srcRect.w * 2;
	m_destRect.h = m_srcRect.h * 2;

}

void GameObject::Render()
{
	SDL_RenderCopy(m_renderer, m_objectTexture, &m_srcRect, &m_destRect);
}
