#pragma once
#include <SDL.h>


struct AppearanceSettings
{
	SDL_Color normalColour = { 255, 255, 255, 255 };
	SDL_Color highlightColour = { 255, 0, 0, 255 };
	int fontSize = 24;

	static AppearanceSettings& Get()
	{
		static AppearanceSettings instance;
		return instance;
	}
};