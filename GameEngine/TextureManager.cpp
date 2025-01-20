#include "TextureManager.h"
#include "AccessibilityManager.h"
#include <iostream>

SDL_Texture* TextureManager::LoadTexture(const char* filePath)
{
    std::string path = filePath;

    if (AccessibilityManager::IsHighContrastEnabled())
    {
        std::string highContrastPath = "High Contrast/"+ path;
        SDL_Surface* tmpSurface = IMG_Load(highContrastPath.c_str());
        SDL_Texture* highContrastTexture = SDL_CreateTextureFromSurface(Game::m_renderer, tmpSurface);
        SDL_FreeSurface(tmpSurface);

        return highContrastTexture;
    }

    SDL_Surface* tmpSurface = IMG_Load(filePath);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::m_renderer, tmpSurface);
    SDL_FreeSurface(tmpSurface);

    return texture;
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip)
{
    SDL_RenderCopyEx(Game::m_renderer, texture, &srcRect, &destRect, NULL, NULL, flip);
}
