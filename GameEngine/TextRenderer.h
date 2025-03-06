#pragma once

#include <string>
#include "SDL.h"
#include "SDL_ttf.h"
#include <iostream>

inline void DrawText(SDL_Renderer* renderer, const std::string& text, int x, int y, TTF_Font* font, SDL_Color color)
{
    // Render text to an SDL_Surface using blended rendering.
    SDL_Surface* surface = TTF_RenderText_Blended(font, text.c_str(), color);
    if (!surface) {
        std::cerr << "Failed to render text surface: " << TTF_GetError() << std::endl;
        return;
    }

    // Create a texture from the surface.
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        std::cerr << "Failed to create text texture: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dstRect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &dstRect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
