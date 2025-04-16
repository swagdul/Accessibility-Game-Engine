#include "AccessibilityMenu.h"
#include "ECS.h"
#include "Components.h"
#include "TextRenderer.h"
#include "AppearanceSettings.h"
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

extern Manager g_manager;

AccessibilityMenu::AccessibilityMenu(Game* game, SDL_Renderer* renderer)
	: m_game(game), m_renderer(renderer), m_isRunning(true), m_selectedIndex(0), m_state(AccessibilityMenuState::MainMenu), m_fontSize(AppearanceSettings::Get().fontSize)
{

	m_normalColour = AppearanceSettings::Get().normalColour;
	m_highlightColour = AppearanceSettings::Get().highlightColour;

	m_menuOptions.push_back("List Entities");
	m_menuOptions.push_back("Create Entities");
	m_menuOptions.push_back("Modify Entities");
	m_menuOptions.push_back("Delete Entities");
	m_menuOptions.push_back("Adjust Appearance");
	m_menuOptions.push_back("Exit Menu");

	ScreenReader::Speak(m_menuOptions[m_selectedIndex]);
}

AccessibilityMenu::~AccessibilityMenu()
{
	ClearLogMessages();
}

void AccessibilityMenu::AddLogMessage(const std::string& message)
{
	m_logMessages.push_back(message);

	if (m_logMessages.size() > 10)
	{
		m_logMessages.erase(m_logMessages.begin());
	}
}

void AccessibilityMenu::ClearLogMessages()
{
	m_logMessages.clear();
}

std::string AccessibilityMenu::TextInput()
{
	SDL_StartTextInput();
	std::string textInput;
	bool done = false;

	TTF_Font* font = TTF_OpenFont("Assets/arial.ttf", m_fontSize);
	if (!font) {
		std::cerr << "AccessibilityMenu::TextInput: Failed to load font: " << TTF_GetError() << std::endl;
		return "";
	}

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

		DrawText(m_renderer, "Enter Text: ", 50, 500, font, m_normalColour);
		DrawText(m_renderer, textInput, 50, 550, font, m_normalColour);

		SDL_RenderPresent(m_renderer);
		SDL_Delay(16);
	}

	SDL_StopTextInput();
	TTF_CloseFont(font);

	return textInput;
}

int AccessibilityMenu::GetIntInput() {
	std::string input = TextInput(); 
	try {
		return std::stoi(input);
	}
	catch (...) {
		AddTextToMenu("Invalid input. Using default value 0.");
		return 0;
	}
}

float AccessibilityMenu::GetFloatInput() {
	std::string input = TextInput();
	try {
		return std::stof(input);
	}
	catch (...) {
		AddTextToMenu("Invalid input. Using default value 0.0.");
		return 0.0f;
	}
}

void AccessibilityMenu::AddTextToMenu(std::string text)
{
	AddLogMessage(text);
	RenderSubMenu();
	SDL_RenderPresent(m_renderer);
	ScreenReader::Speak(text);
}

void AccessibilityMenu::Run()
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

void AccessibilityMenu::HandleEvent(SDL_Event& event)
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
				m_state = AccessibilityMenuState::CreateEntity;
				CreateEntity();
				m_state = AccessibilityMenuState::MainMenu;
			}
			else if (m_menuOptions[m_selectedIndex] == "Modify Entities")
			{
				ModifyEntity();
			}
			else if (m_menuOptions[m_selectedIndex] == "Delete Entities")
			{
				DeleteEntity();
			}
			else if (m_menuOptions[m_selectedIndex] == "Adjust Appearance")
			{
				AdjustAppearance();
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

void AccessibilityMenu::RenderMainMenu()
{
	int windowWidth = 0, windowHeight = 0;
	SDL_GetRendererOutputSize(m_renderer, &windowWidth, &windowHeight);

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
	SDL_RenderFillRect(m_renderer, nullptr);

	TTF_Font* font = TTF_OpenFont("Assets/arial.ttf", m_fontSize);
	if (!font) {
		std::cerr << "AccessibilityMenu::Render: Failed to load font: " << TTF_GetError() << std::endl;
		return;
	}

	int startY = 100;
	int spacingY = 50;

	for (size_t i = 0; i < m_menuOptions.size(); i++)
	{
		SDL_Color color = (static_cast<int>(i) == m_selectedIndex) ? m_highlightColour : m_normalColour;

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
		DrawText(m_renderer, m_logMessages[i], 100, logStartY + static_cast<int>(i) * logSpacingY, font, m_normalColour);
	}

	SDL_RenderPresent(m_renderer);

	TTF_CloseFont(font);
}

void AccessibilityMenu::RenderSubMenu()
{
	int windowWidth = 0, windowHeight = 0;
	SDL_GetRendererOutputSize(m_renderer, &windowWidth, &windowHeight);

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_NONE);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
	SDL_RenderFillRect(m_renderer, nullptr);

	//SDL_Color messageColor = { 200, 200, 200, 255 };
	int msgY = 20;

	TTF_Font* smallFont = TTF_OpenFont("Assets/arial.ttf", 24);
	if (smallFont)
	{
		for (size_t i = 0; i < m_logMessages.size(); ++i) {
			DrawText(m_renderer, m_logMessages[i], 20, msgY + static_cast<int>(i) * 30, smallFont, m_normalColour);
		}
	}

	SDL_RenderPresent(m_renderer);

	TTF_CloseFont(smallFont);
}

void AccessibilityMenu::CreateEntity()
{
	ClearLogMessages();

	AddTextToMenu("Creating new entity.");

	/////////////////////////////////////////////////////////////////////////////////////////
	//Prompt to add name and group
	/////////////////////////////////////////////////////////////////////////////////////////

	AddTextToMenu("Enter the name of the entity.");
	std::string name = TextInput();
	Entity& newEntity = g_manager.addEntity();
	newEntity.setName(name);
	newEntity.setDebugCreated(true);
	AddTextToMenu("Entity \"" + name + "\" created.");
	AddTextToMenu("What group would you like to add this Entity to?");

	//Currently hardcoding, adapt later 
	AddTextToMenu("Available groups:");
	AddTextToMenu("Select 0 for Enemies.");
	AddTextToMenu("Select 1 for Players.");
	AddTextToMenu("Select 2 for Maps.");
	AddTextToMenu("Select 3 for Colliders.");
	AddTextToMenu("Select 4 for Objects.");
	AddTextToMenu("Select 5 for Projectiles.");

	int groupChoice = GetIntInput();

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

	AddTextToMenu("Adding TransformComponent.");
	AddTextToMenu("This will give the entity a position, height, width, and scale.");
	AddTextToMenu("The height and width will be based on the texture.");

	float xPos = 0.0f, yPos = 0.0f, scale = 1.0f;
	int height = 128, width = 128;

	AddTextToMenu("Enter X position (float): ");
	xPos = GetFloatInput();

	AddTextToMenu("Enter Y position (float): ");
	yPos = GetFloatInput();

	AddTextToMenu("Enter height (int): ");
	height = GetIntInput();

	AddTextToMenu("Enter width (int): ");
	width = GetIntInput();

	AddTextToMenu("Enter scale (float): ");
	scale = GetFloatInput();

	newEntity.addComponent<TransformComponent>(xPos, yPos, height, width, scale);
	AddTextToMenu("Transform component added.");
	

	/////////////////////////////////////////////////////////////////////////////////////////
	//Prompt to add SpriteComponent	
	/////////////////////////////////////////////////////////////////////////////////////////
	
	AddTextToMenu("Adding a SpriteComponent.");

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
	animated = GetIntInput();

	AddTextToMenu("Do you want the sprite to be flipped horizontally? (1 for yes, 0 for no): ");
	bool flipped = GetIntInput();

	if (animated)
	{
		if (textureID == "DefaultTexture")
		{
			newEntity.addComponent<SpriteComponent>(textureID, animated, 6, 100, flipped);
			AddTextToMenu("Sprite component added.");
		}
		else
		{
			AddTextToMenu("Enter the number of frames your sprite animation has: ");
			int frames = GetIntInput();

			AddTextToMenu("Enter the speed of your sprite animation as an int: ");
			int speed = GetIntInput();

			newEntity.addComponent<SpriteComponent>(textureID, animated, frames, speed, flipped);
			AddTextToMenu("Sprite component added.");
		}
	}
	else
	{
		newEntity.addComponent<SpriteComponent>(textureID, animated, flipped);
		AddTextToMenu("Sprite component added.");
	}

	

	/////////////////////////////////////////////////////////////////////////////////////////
	//Prompt to add ColliderComponent
	/////////////////////////////////////////////////////////////////////////////////////////

	AddTextToMenu("Add a ColliderComponent? (1 for yes, 0 for no): ");

	int addCollider = GetIntInput();

	if (addCollider == 1)
	{
		std::string colliderTag;
		AddTextToMenu("Enter a name for the collider tag: ");
		colliderTag = TextInput();
		newEntity.addComponent<ColliderComponent>(colliderTag);
		AddTextToMenu("Collider component added.");
	}

	/////////////////////////////////////////////////////////////////////////////////////////
	//Prompt to add HealthComponent
	/////////////////////////////////////////////////////////////////////////////////////////

	AddTextToMenu("Add a HealthComponent? (1 for yes, 0 for no): ");

	int addHealth = GetIntInput();

	if (addHealth == 1)
	{
		int health = 100;
		bool isPlayer = false;
		AddTextToMenu("Enter health (int): ");
		health = GetIntInput();
		if (newEntity.hasGroup(Game::g_groupLabels::Players))
		{
			isPlayer = true;
		}
		newEntity.addComponent<HealthComponent>(health, isPlayer);
		AddTextToMenu("Health component added.");
	}

	AddTextToMenu("Returning to main menu.");

	g_manager.refresh();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	ClearLogMessages();
}

void AccessibilityMenu::ModifyEntity()
{
	ClearLogMessages();
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
	int entityIndex = GetIntInput();

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

	int modChoice = GetIntInput();

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
			float newX = GetFloatInput();

			AddTextToMenu("Enter new Y position (float): ");
			float newY = GetFloatInput();

			AddTextToMenu("Enter new width (int): ");
			int newWidth = GetIntInput();

			AddTextToMenu("Enter new height (int): ");
			int newHeight = GetIntInput();

			AddTextToMenu("Enter new scale (float): ");
			float newScale = GetIntInput();

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
			int isAnimated = GetIntInput();
			if (isAnimated == 1)
			{
				sc.enableAnimation();
			}

			AddTextToMenu("Do you want the sprite to be flipped horizontally? (1 for yes, 0 for no): ");
			bool flipped = GetIntInput();
			sc.setFlip(flipped);

			AddTextToMenu("Enter the number of frames your sprite animation has: ");
			int frames = GetIntInput();
			sc.setAnimationFrames(frames);

			AddTextToMenu("Enter the speed of your sprite animation as an int: ");
			int speed = GetIntInput();
			sc.setAnimationSpeed(speed);

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
			int newHealth = GetIntInput();

			AddTextToMenu("Make entity a player? (1 for yes, 0 for no): ");
			bool isPlayer = GetIntInput();

			hc.setMaxHealth(newHealth);
			hc.setIfPlayer(isPlayer);

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

		int addChoice = GetIntInput();

		if (addChoice == 1)
		{
			if (selectedEntity->hasComponent<TransformComponent>())
			{
				AddTextToMenu("Entity already has a Transform component.");
			}
			else
			{
				AddTextToMenu("Enter X position (float): ");
				float xPos = GetFloatInput();

				AddTextToMenu("Enter Y position (float): ");
				float yPos = GetFloatInput();

				AddTextToMenu("Enter height (int): ");
				int height = GetIntInput();

				AddTextToMenu("Enter width (int): ");
				int width = GetIntInput();

				AddTextToMenu("Enter scale (float): ");
				float scale = GetFloatInput();

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
				int animated = GetIntInput();

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
				int health = GetIntInput();

				AddTextToMenu("Is this a player entity? (1 for yes, 0 for no): ");
				bool isPlayer = GetIntInput();

				selectedEntity->addComponent<HealthComponent>(health, isPlayer);

				AddTextToMenu("Health component added.");
			}
		}
	}
	else
	{
		AddTextToMenu("Invalid modification choice.");
	}

	AddTextToMenu("Returning to main menu.");

	g_manager.refresh();
	ClearLogMessages();
}


void AccessibilityMenu::ListEntities()
{
	ClearLogMessages();
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

	AddTextToMenu("Returning to main menu.");

	ClearLogMessages();

	std::this_thread::sleep_for(std::chrono::seconds(2));
}

void AccessibilityMenu::AdjustAppearance()
{
	m_normalColour = AppearanceSettings::Get().normalColour;
	m_highlightColour = AppearanceSettings::Get().highlightColour;
	m_fontSize = AppearanceSettings::Get().fontSize;

	ClearLogMessages();
	
	AddTextToMenu("Adjusting appearance.");

	AddTextToMenu("Do you want to change the text colour? (1 for yes, 0 for no): ");
	int changeColour = GetIntInput();

	if (changeColour == 1)
	{
		int r = 0, g = 0, b = 0, a = 0;

		AddTextToMenu("Enter red value (0-255): ");
		r = GetIntInput();

		AddTextToMenu("Enter green value (0-255): ");
		g = GetIntInput();

		AddTextToMenu("Enter blue value (0-255): ");
		b = GetIntInput();

		AddTextToMenu("Enter alpha value (0-255): ");
		a = GetIntInput();

		m_normalColour = { static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), static_cast<Uint8>(a) };
	}

	AddTextToMenu("Do you want to change the highlight colour? (1 for yes, 0 for no): ");
	int changeHighlight = GetIntInput();

	if (changeHighlight == 1)
	{
		int r = 0, g = 0, b = 0, a = 0;

		AddTextToMenu("Enter red value (0-255): ");
		r = GetIntInput();

		AddTextToMenu("Enter green value (0-255): ");
		g = GetIntInput();

		AddTextToMenu("Enter blue value (0-255): ");
		b = GetIntInput();

		AddTextToMenu("Enter alpha value (0-255): ");
		a = GetIntInput();

		m_highlightColour = { static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b), static_cast<Uint8>(a) };
	}

	AddTextToMenu("Do you want to change the font size? (1 for yes, 0 for no): ");	
	int changeFontSize = GetIntInput();

	if (changeFontSize == 1)
	{
		AddTextToMenu("Enter new font size: ");
		m_fontSize = GetIntInput();
	}

	AddTextToMenu("Returning to main menu.");

	g_manager.refresh();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	ClearLogMessages();
}

void AccessibilityMenu::DeleteEntity()
{
	ClearLogMessages();

	std::vector<Entity*> entities = g_manager.getEntities();

	if (entities.empty())
	{
		AddTextToMenu("No entities available to delete.");
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

	AddTextToMenu("Enter the index of the entity to delete: ");

	int entityIndex = GetIntInput();
	if (entityIndex < 0 || entityIndex >= static_cast<int>(entities.size())) {
		AddTextToMenu("Invalid entity index.");
		return;
	}

	if (entityIndex < 0 || entityIndex >= static_cast<int>(entities.size()))
	{
		AddTextToMenu("Invalid index. No entity deleted.");
		ScreenReader::Speak("Invalid index. No entity deleted.");
	}
	else
	{
		entities[entityIndex]->destroy();
		AddTextToMenu("Entity " + std::to_string(entityIndex) + " deleted.");
		ScreenReader::Speak("Entity deleted.");
	}

	Entity* selectedEntity = entities[entityIndex];
	selectedEntity->destroy();

	AppearanceSettings::Get().normalColour = m_normalColour;
	AppearanceSettings::Get().highlightColour = m_highlightColour;
	AppearanceSettings::Get().fontSize = m_fontSize;

	AddTextToMenu("Entity deleted.");

	g_manager.refresh();

	std::this_thread::sleep_for(std::chrono::seconds(2));
	ClearLogMessages();
}



