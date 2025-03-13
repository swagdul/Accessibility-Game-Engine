#include "MenuSystem.h"

MenuSystem::MenuSystem(Game* game, SDL_Renderer* renderer, TTF_Font* font, const std::vector<std::string>& options)
	: m_game(game), m_renderer(renderer), m_font(font), m_menuLabels(options), m_selected(options.size(), false)
{

	for (const auto& label : options)
	{
		SDL_Surface* surface = TTF_RenderText_Solid(m_font, label.c_str(), m_menuColours[0]);
		m_menuSurfaces.push_back(surface);
		m_menuTextures.push_back(SDL_CreateTextureFromSurface(m_renderer, surface));

		SDL_Rect position;
		position.x = 400 - surface->w / 2;
		position.y = 200 + static_cast<int>(m_menuSurfaces.size()) * 50;
		position.w = surface->w;
		position.h = surface->h;
		m_menuPositions.push_back(position);
	}
}

MenuSystem::~MenuSystem()
{
	Cleanup();
}

int MenuSystem::DisplayMenu()
{
	SDL_Event event;
	int xMouse, yMouse;

	while (m_game->isRunning())
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				Cleanup();
				m_game->clean();
				return -1;
			}

			if (event.type == SDL_MOUSEMOTION)
			{
				xMouse = event.motion.x;
				yMouse = event.motion.y;

				for (size_t i = 0; i < m_menuLabels.size(); ++i)
				{
					if (xMouse >= m_menuPositions[i].x && xMouse <= m_menuPositions[i].x + m_menuPositions[i].w &&
						yMouse >= m_menuPositions[i].y && yMouse <= m_menuPositions[i].y + m_menuPositions[i].h)
					{
						if (!m_selected[i])
						{
							m_selected[i] = true;
							SDL_DestroyTexture(m_menuTextures[i]);
							SDL_Surface* surface = TTF_RenderText_Solid(m_font, m_menuLabels[i].c_str(), m_menuColours[1]);
							m_menuTextures[i] = SDL_CreateTextureFromSurface(m_renderer, surface);
							SDL_FreeSurface(surface);
						}
					}
					else if (m_selected[i])
					{
						m_selected[i] = false;
						SDL_DestroyTexture(m_menuTextures[i]);
						SDL_Surface* surface = TTF_RenderText_Solid(m_font, m_menuLabels[i].c_str(), m_menuColours[0]);
						m_menuTextures[i] = SDL_CreateTextureFromSurface(m_renderer, surface);
						SDL_FreeSurface(surface);
					}

				}
			}

			if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				xMouse = event.button.x;
				yMouse = event.button.y;
				
				for (size_t i = 0; i < m_menuLabels.size(); ++i)
				{
					if (xMouse >= m_menuPositions[i].x && xMouse <= m_menuPositions[i].x + m_menuPositions[i].w &&
						yMouse >= m_menuPositions[i].y && yMouse <= m_menuPositions[i].y + m_menuPositions[i].h) {
						

						if (i == m_menuLabels.size() - 1) 
						{  
							Cleanup();
							m_game->clean();  
							return -1;
						}

						return static_cast<int>(i);
					}
				}
			}

			if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    m_selectedIndex--;
                    if (m_selectedIndex < 0)
                        m_selectedIndex = m_menuLabels.size() - 1;
                    // Optionally update the textures to reflect new selection:
                    for (size_t i = 0; i < m_menuLabels.size(); ++i)
                    {
                        bool selected = (i == static_cast<size_t>(m_selectedIndex));
                        m_selected[i] = selected;
                        SDL_DestroyTexture(m_menuTextures[i]);
                        SDL_Surface* surface = TTF_RenderText_Solid(m_font, m_menuLabels[i].c_str(),
                            selected ? m_menuColours[1] : m_menuColours[0]);
                        m_menuTextures[i] = SDL_CreateTextureFromSurface(m_renderer, surface);
                        SDL_FreeSurface(surface);
                    }
                    break;
                case SDLK_DOWN:
                    m_selectedIndex++;
                    if (m_selectedIndex >= static_cast<int>(m_menuLabels.size()))
                        m_selectedIndex = 0;
                    for (size_t i = 0; i < m_menuLabels.size(); ++i)
                    {
                        bool selected = (i == static_cast<size_t>(m_selectedIndex));
                        m_selected[i] = selected;
                        SDL_DestroyTexture(m_menuTextures[i]);
                        SDL_Surface* surface = TTF_RenderText_Solid(m_font, m_menuLabels[i].c_str(),
                            selected ? m_menuColours[1] : m_menuColours[0]);
                        m_menuTextures[i] = SDL_CreateTextureFromSurface(m_renderer, surface);
                        SDL_FreeSurface(surface);
                    }
                    break;
                case SDLK_RETURN:
                    if (m_selectedIndex == static_cast<int>(m_menuLabels.size()) - 1)
                    {
                        Cleanup();
                        m_game->clean();
                        return -1;
                    }
                    return m_selectedIndex;
                    break;
                case SDLK_ESCAPE:
                    Cleanup();
                    m_game->clean();
                    return -1;
                }
            }
        }

		if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
			Cleanup();
			return -1;
		}
		

		RenderMenu();
	}
}

void MenuSystem::RenderMenu()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 225);
	SDL_RenderClear(m_renderer);
	for (size_t i = 0; i < m_menuTextures.size(); ++i)
	{
		SDL_RenderCopy(m_renderer, m_menuTextures[i], NULL, &m_menuPositions[i]);
	}
	SDL_RenderPresent(m_renderer);
}

void MenuSystem::Cleanup()
{
	if (m_menuTextures.empty() && m_menuSurfaces.empty()) {
		return;  
	}

	for (auto& texture : m_menuTextures) {
		if (texture) {
			SDL_DestroyTexture(texture);
		}
	}
	m_menuTextures.clear();

	for (auto& surface : m_menuSurfaces) {
		if (surface) {
			SDL_FreeSurface(surface);
		}
	}
	m_menuSurfaces.clear();
}



