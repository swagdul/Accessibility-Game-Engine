#include "DebugMenu.h"
#include "ECS.h"
#include "Components.h"
#include "TextRenderer.h"
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

extern Manager g_manager;

DebugMenu::DebugMenu(Game* game, SDL_Renderer* renderer)
	: m_game(game), m_renderer(renderer), m_isRunning(true), m_selectedIndex(0), m_state(DebugMenuState::MainMenu)
{
	m_menuOptions.push_back("List Entities");
	m_menuOptions.push_back("Create Entities");
	m_menuOptions.push_back("Modify Entities");
	m_menuOptions.push_back("Exit Menu");

	ScreenReader::Speak(m_menuOptions[m_selectedIndex]);
}

DebugMenu::~DebugMenu()
{
	ClearLogMessages();
}

void DebugMenu::AddLogMessage(const std::string& message)
{
	m_logMessages.push_back(message);

	if (m_logMessages.size() > 10)
	{
		m_logMessages.erase(m_logMessages.begin());
	}
}

void DebugMenu::ClearLogMessages()
{
	m_logMessages.clear();
}

std::string DebugMenu::TextInput()
{
	SDL_StartTextInput();
	std::string textInput;
	bool done = false;

	TTF_Font* font = TTF_OpenFont("Assets/arial.ttf", 24);
	if (!font) {
		std::cerr << "DebugMenu::TextInput: Failed to load font: " << TTF_GetError() << std::endl;
		return "";
	}

	SDL_Color textColour = { 255, 255, 255, 255 };

	int windowWidth = 0, windowHeight = 0;
	SDL_GetRendererOutputSize(m_renderer, &windowWidth, &windowHeight);

	const int inputAreaHeight = 80;

	while (!done)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				done = true;
			}
			else if (event.type == SDL_TEXTINPUT)
			{
				textInput += event.text.text;
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_RETURN)
				{
					done = true;
				}
				else if (event.key.keysym.sym == SDLK_BACKSPACE && !textInput.empty())
				{
					textInput.pop_back();
				}
			}
		}

		RenderSubMenu();

		SDL_Rect inpuytRect = { 0, windowHeight - inputAreaHeight, windowWidth, inputAreaHeight };
		SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
		SDL_RenderFillRect(m_renderer, &inpuytRect);

		DrawText(m_renderer, "Enter Text: ", 50, 500, font, textColour);
		DrawText(m_renderer, textInput, 50, 550, font, textColour);

		SDL_RenderPresent(m_renderer);
		SDL_Delay(16);
	}

	SDL_StopTextInput();
	TTF_CloseFont(font);

	return textInput;
}

void DebugMenu::AddTextToMenu(std::string text)
{
	AddLogMessage(text);
	RenderSubMenu();
	SDL_RenderPresent(m_renderer);
	ScreenReader::Speak(text);
}

void DebugMenu::Run()
{
	SDL_Event event;

	while (m_isRunning && m_game->isRunning())
	{
		while (SDL_PollEvent(&event))
		{
			HandleEvent(event);
		}
		RenderMainMenu();
	}
}

void DebugMenu::HandleEvent(SDL_Event& event)
{
	if (event.type == SDL_QUIT)
	{
		m_isRunning = false;
		m_game->clean();
		return;
	}

	if (event.type == SDL_KEYDOWN)
	{
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:

			m_selectedIndex--;

			if (m_selectedIndex < 0)
			{
				m_selectedIndex = m_menuOptions.size() - 1;
			}

			ScreenReader::Speak(m_menuOptions[m_selectedIndex]);

			break;

		case SDLK_DOWN:

			m_selectedIndex++;

			if (m_selectedIndex >= static_cast<int>(m_menuOptions.size()))
			{
				m_selectedIndex = 0;
			}

			ScreenReader::Speak(m_menuOptions[m_selectedIndex]);

			break;

		case SDLK_RETURN:

			if (m_menuOptions[m_selectedIndex] == "List Entities")
			{
				ListEntities();
			}
			else if (m_menuOptions[m_selectedIndex] == "Create Entities")
			{
				m_state = DebugMenuState::CreateEntity;
				CreateEntity();
				m_state = DebugMenuState::MainMenu;
			}
			else if (m_menuOptions[m_selectedIndex] == "Modify Entities")
			{
				ModifyEntity();
			}
			else if (m_menuOptions[m_selectedIndex] == "Exit Menu")
			{
				m_isRunning = false;
			}
			break;

		case SDLK_ESCAPE:

			m_isRunning = false;
			break;

		}
	}
}

void DebugMenu::RenderMainMenu()
{
	int windowWidth = 0, windowHeight = 0;
	SDL_GetRendererOutputSize(m_renderer, &windowWidth, &windowHeight);

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
	SDL_RenderFillRect(m_renderer, nullptr);

	TTF_Font* font = TTF_OpenFont("Assets/arial.ttf", 24);
	if (!font) {
		std::cerr << "DebugMenu::Render: Failed to load font: " << TTF_GetError() << std::endl;
		return;
	}

	SDL_Color normalColor = { 255, 255, 255, 255 };
	SDL_Color highlightColor = { 255, 0, 0, 255 };

	int startY = 100;
	int spacingY = 50;

	for (size_t i = 0; i < m_menuOptions.size(); i++)
	{
		SDL_Color color = (static_cast<int>(i) == m_selectedIndex) ? highlightColor : normalColor;

		SDL_Surface* textSurface = TTF_RenderText_Blended(font, m_menuOptions[i].c_str(), color);
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_renderer, textSurface);

		SDL_Rect destRect;
		destRect.w = textSurface->w;
		destRect.h = textSurface->h;
		destRect.x = (windowWidth - destRect.w) / 2;
		destRect.y = startY + static_cast<int>(i) * spacingY;

		SDL_RenderCopy(m_renderer, textTexture, nullptr, &destRect);

		SDL_DestroyTexture(textTexture);
		SDL_FreeSurface(textSurface);
	}

	int logStartY = 100;
	int logSpacingY = 30;

	for (size_t i = 0; i < m_logMessages.size(); ++i)
	{
		DrawText(m_renderer, m_logMessages[i], 100, logStartY + static_cast<int>(i) * logSpacingY, font, normalColor);
	}

	SDL_RenderPresent(m_renderer);

	TTF_CloseFont(font);
}

void DebugMenu::RenderSubMenu()
{
	int windowWidth = 0, windowHeight = 0;
	SDL_GetRendererOutputSize(m_renderer, &windowWidth, &windowHeight);

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
	SDL_RenderFillRect(m_renderer, nullptr);

	SDL_Color messageColor = { 200, 200, 200, 255 };
	int msgY = 20;

	TTF_Font* smallFont = TTF_OpenFont("Assets/arial.ttf", 24);
	if (smallFont)
	{
		for (size_t i = 0; i < m_logMessages.size(); ++i) {
			DrawText(m_renderer, m_logMessages[i], 20, msgY + static_cast<int>(i) * 30, smallFont, messageColor);
		}
	}

	SDL_RenderPresent(m_renderer);

	TTF_CloseFont(smallFont);
}

void DebugMenu::CreateEntity()
{
	ClearLogMessages();

	RenderSubMenu();
	SDL_RenderPresent(m_renderer);

	AddTextToMenu("Creating new entity.");

	/////////////////////////////////////////////////////////////////////////////////////////
	//Prompt to add name and group
	/////////////////////////////////////////////////////////////////////////////////////////

	AddTextToMenu("Enter the name of the entity.");
	std::string name = TextInput();
	Entity& newEntity = g_manager.addEntity();
	newEntity.setName(name);
	AddTextToMenu("Entity \"" + name + "\" created.");
	AddTextToMenu("What group would you like to add this Entity to?");

	//Currently hardcoding, adapt later 
	AddTextToMenu("Available groups.");
	AddTextToMenu("Select 0 for Enemies.");
	AddTextToMenu("Select 1 for Players.");
	AddTextToMenu("Select 2 for Maps.");
	AddTextToMenu("Select 3 for Colliders.");
	AddTextToMenu("Select 4 for Objects.");
	AddTextToMenu("Select 5 for Projectiles.");

	int groupChoice = std::stoi(TextInput());

	if (groupChoice < 0 || groupChoice > 5)
	{
		AddTextToMenu("Invalid group choice. Defaulting to Object.");
		groupChoice = 0; //defualt to enemy
		return;
	}

	newEntity.addGroup(groupChoice);

	/////////////////////////////////////////////////////////////////////////////////////////
	//Prompt to add TransformComponent
	/////////////////////////////////////////////////////////////////////////////////////////

	AddTextToMenu("Add a TransformComponent? (1 for yes, 0 for no): ");

	int addTransform = std::stoi(TextInput());

	if (addTransform == 1)
	{
		float xPos = 0.0f, yPos = 0.0f, scale = 1.0f;
		int height = 128, width = 128;

		AddTextToMenu("Enter X position (float): ");
		xPos = std::stof(TextInput());

		AddTextToMenu("Enter Y position (float): ");
		yPos = std::stof(TextInput());

		AddTextToMenu("Enter height (int): ");
		height = std::stoi(TextInput());

		AddTextToMenu("Enter width (int): ");
		width = std::stoi(TextInput());

		AddTextToMenu("Enter scale (float): ");
		scale = std::stof(TextInput());	

		newEntity.addComponent<TransformComponent>(xPos, yPos, height, width, scale);
		AddTextToMenu("Transform component added.");
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//Prompt to add SpriteComponent	
	/////////////////////////////////////////////////////////////////////////////////////////
	
	AddTextToMenu("Add a SpriteComponent? (1 for yes, 0 for no): ");

	int addSprite = std::stoi(TextInput());

	if (addSprite == 1)
	{
		std::string textureID;
		bool animated = false;

		AddTextToMenu("Enter texture ID: ");
		AddTextToMenu("Alternatively, press enter to use the default texture ID.");

		textureID = TextInput();

		if (textureID.empty())
		{
			textureID = "DefaultTexture";
		}

		AddTextToMenu("Is the sprite animated? (1 for yes, 0 for no): ");
		animated = std::stoi(TextInput());

		newEntity.addComponent<SpriteComponent>(textureID, animated);
		AddTextToMenu("Sprite component added.");
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//Prompt to add ColliderComponent
	/////////////////////////////////////////////////////////////////////////////////////////

	AddTextToMenu("Add a ColliderComponent? (1 for yes, 0 for no): ");

	int addCollider = std::stoi(TextInput());

	if (addCollider == 1)
	{
		std::string colliderTag;
		AddTextToMenu("Enter collider tag: ");
		colliderTag = TextInput();
		newEntity.addComponent<ColliderComponent>(colliderTag);
		AddTextToMenu("Collider component added.");
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//Prompt to add HealthComponent
	/////////////////////////////////////////////////////////////////////////////////////////

	AddTextToMenu("Add a HealthComponent? (1 for yes, 0 for no): ");

	int addHealth = std::stoi(TextInput());

	if (addHealth == 1)
	{
		int health = 100;
		bool isPlayer = false;
		AddTextToMenu("Enter health (int): ");
		health = std::stoi(TextInput());
		AddTextToMenu("Is this a player entity? (1 for yes, 0 for no): ");
		isPlayer = std::stoi(TextInput());
		newEntity.addComponent<HealthComponent>(health, isPlayer);
		AddTextToMenu("Health component added.");
	}

	g_manager.refresh();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	ClearLogMessages();
}

void DebugMenu::ModifyEntity()
{
	std::vector<Entity*> entities = g_manager.getEntities();

	if (entities.empty())
	{
		AddTextToMenu("No entities available to modify.");
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return;
	}

	AddTextToMenu("Available entities:");
	for (size_t i = 0; i < entities.size(); i++) {
		if (entities[i]->getName() != "Unnamed")
		{
			AddTextToMenu("Entity " + std::to_string(i) + ": " + entities[i]->getName());
		}
	}

	AddTextToMenu("Enter the index of the entity to modify: ");
	int entityIndex = std::stoi(TextInput());

	if (entityIndex < 0 || entityIndex >= static_cast<int>(entities.size())) {
		AddTextToMenu("Invalid entity index.");
		return;
	}

	Entity* selectedEntity = entities[entityIndex];

	AddTextToMenu("Modification Options:");
	AddTextToMenu("1: Modify TransformComponent");
	AddTextToMenu("2: Modify SpriteComponent");
	AddTextToMenu("3: Modify ColliderComponent");
	AddTextToMenu("4: Modify HealthComponent");
	AddTextToMenu("5: Add missing component");
	AddTextToMenu("Enter your choice: ");

	int modChoice = std::stoi(TextInput());

	if (modChoice == 1)
	{
		if (selectedEntity->hasComponent<TransformComponent>())
		{
			TransformComponent& tc = selectedEntity->getComponent<TransformComponent>();

			AddTextToMenu("Current Position: (" + std::to_string(tc.m_position.m_x) + ", " + std::to_string(tc.m_position.m_y) + ")");
			AddTextToMenu("Current Width: " + std::to_string(tc.m_width));
			AddTextToMenu("Current Height: " + std::to_string(tc.m_height));
			AddTextToMenu("Current Scale: " + std::to_string(tc.m_scale));

			AddTextToMenu("Enter new X position (float): ");
			float newX = std::stof(TextInput());

			AddTextToMenu("Enter new Y position (float): ");
			float newY = std::stof(TextInput());

			AddTextToMenu("Enter new width (int): ");
			int newWidth = std::stoi(TextInput());

			AddTextToMenu("Enter new height (int): ");
			int newHeight = std::stoi(TextInput());

			AddTextToMenu("Enter new scale (float): ");
			float newScale = std::stof(TextInput());

			tc.m_position.m_x = newX;
			tc.m_position.m_y = newY;
			tc.m_width = newWidth;
			tc.m_height = newHeight;
			tc.m_scale = newScale;

			AddTextToMenu("Transform component updated.");
		}
		else
		{
			AddTextToMenu("Entity does not have a Transform component.");
		}
	}
	else if (modChoice == 2)
	{
		if (selectedEntity->hasComponent<SpriteComponent>())
		{
			SpriteComponent& sc = selectedEntity->getComponent<SpriteComponent>();

			AddTextToMenu("Enter new texture ID: ");
			std::string newTexture = TextInput();
			if (newTexture.empty())
			{
				newTexture = "DefaultTexture";
			}
			sc.setTexture(newTexture);


			AddTextToMenu("Is the sprite animated? (1 for yes, 0 for no): ");
			int isAnimated = std::stoi(TextInput());
			if (isAnimated == 1)
			{
				sc.enableAnimation();
			}

			AddTextToMenu("Sprite component updated.");
		}
		else
		{
			AddTextToMenu("Entity does not have a Sprite component.");
		}
	}
	else if (modChoice == 3)
	{
		if (selectedEntity->hasComponent<ColliderComponent>())
		{
			ColliderComponent& cc = selectedEntity->getComponent<ColliderComponent>();

			AddTextToMenu("Add new tag: ");
			std::string newTag = TextInput();

			cc.m_tag = newTag;

			AddTextToMenu("Collider component updated.");
		}
		else
		{
			AddTextToMenu("Entity does not have a Collider component.");
		}
	}
	else if (modChoice == 4)
	{
		if (selectedEntity->hasComponent<HealthComponent>())
		{
			HealthComponent& hc = selectedEntity->getComponent<HealthComponent>();

			AddTextToMenu("Enter new health value: ");
			int newHealth = std::stoi(TextInput());

			AddTextToMenu("Make entity a player? (1 for yes, 0 for no): ");
			bool isPlayer = std::stoi(TextInput());

			hc.setMaxHealth(newHealth);
			hc.isPlayer(isPlayer);

			AddTextToMenu("Health component updated.");
		}
		else
		{
			AddTextToMenu("Entity does not have a Health component.");
		}
	}
	else if (modChoice == 5)
	{
		AddTextToMenu("Select component to add: ");
		AddTextToMenu("1: TransformComponent");
		AddTextToMenu("2: SpriteComponent");
		AddTextToMenu("3: ColliderComponent");
		AddTextToMenu("4: HealthComponent");

		int addChoice = std::stoi(TextInput());

		if (addChoice == 1)
		{
			if (selectedEntity->hasComponent<TransformComponent>())
			{
				AddTextToMenu("Entity already has a Transform component.");
			}
			else
			{
				AddTextToMenu("Enter X position (float): ");
				float xPos = std::stof(TextInput());

				AddTextToMenu("Enter Y position (float): ");
				float yPos = std::stof(TextInput());

				AddTextToMenu("Enter height (int): ");
				int height = std::stoi(TextInput());

				AddTextToMenu("Enter width (int): ");
				int width = std::stoi(TextInput());

				AddTextToMenu("Enter scale (float): ");
				float scale = std::stof(TextInput());

				selectedEntity->addComponent<TransformComponent>(xPos, yPos, height, width, scale);
		
				AddTextToMenu("Transform component added.");
			}
		}
		else if (addChoice == 2)
		{
			if (selectedEntity->hasComponent<SpriteComponent>())
			{
				AddTextToMenu("Entity already has a Sprite component.");
			}
			else
			{
				AddTextToMenu("Enter texture ID: ");
				AddTextToMenu("Alternatively, press enter to use the default texture ID.");

				std::string textureID = TextInput();
				if (textureID.empty())
				{
					textureID = "DefaultTexture";
				}

				AddTextToMenu("Is the sprite animated? (1 for yes, 0 for no): ");
				int animated = std::stoi(TextInput());				

				selectedEntity->addComponent<SpriteComponent>(textureID, animated);

				AddTextToMenu("Sprite component added.");
			}
		}
		else if (addChoice == 3)
		{
			if (selectedEntity->hasComponent<ColliderComponent>())
			{
				AddTextToMenu("Entity already has a Collider component.");
			}
			else
			{
				AddTextToMenu("Enter collider tag: ");
				std::string colliderTag = TextInput();

				selectedEntity->addComponent<ColliderComponent>(colliderTag);

				AddTextToMenu("Collider component added.");
			}
		}
		else if (addChoice == 4)
		{
			if (selectedEntity->hasComponent<HealthComponent>())
			{
				AddTextToMenu("Entity already has a Health component.");
			}
			else
			{
				AddTextToMenu("Enter health (int): ");
				int health = std::stoi(TextInput());

				AddTextToMenu("Is this a player entity? (1 for yes, 0 for no): ");
				bool isPlayer = std::stoi(TextInput());

				selectedEntity->addComponent<HealthComponent>(health, isPlayer);

				AddTextToMenu("Health component added.");
			}
		}
	}
	else
	{
		AddTextToMenu("Invalid modification choice.");
	}

	g_manager.refresh();
	ClearLogMessages();
}


void DebugMenu::ListEntities()
{
	std::vector<Entity*> entites = g_manager.getEntities();

	if (entites.empty())
	{
		AddTextToMenu("No entities are present.");
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return;
	}

	int unnamedCount = 0;

	AddTextToMenu("There are " + std::to_string(entites.size()) + " entities.");

	for (size_t i = 0; i < entites.size(); i++)
	{
		std::stringstream entityInfo;
		std::string name = entites[i]->getName();
		if (!name.empty() && name != "Unnamed")
		{
			entityInfo << "Entity " << name << " is " << (entites[i]->isActive() ? "active" : "inactive") << ".";
			AddTextToMenu(entityInfo.str());
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
		else
		{
			unnamedCount++;
		}
	}

	AddTextToMenu("There are " + std::to_string(unnamedCount) + " other unnamed entities.");

	ClearLogMessages();

	std::this_thread::sleep_for(std::chrono::seconds(2));
}




