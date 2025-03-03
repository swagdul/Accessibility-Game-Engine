#include "DebugMenu.h"
#include "ECS.h"
#include "Components.h"
#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

extern Manager g_manager;

DebugMenu::DebugMenu(Game* game, SDL_Renderer* renderer)
	: m_game(game), m_renderer(renderer), m_isRunning(true), m_selectedIndex(0)
{
	m_menuOptions.push_back("Create Entities");
	m_menuOptions.push_back("Modify Entities");
	m_menuOptions.push_back("List Entities");
	m_menuOptions.push_back("Exit Debug Menu");

	ScreenReader::Speak(m_menuOptions[m_selectedIndex]);
}

DebugMenu::~DebugMenu()
{
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
		Render();
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

			if (m_menuOptions[m_selectedIndex] == "Create Entities")
			{
				CreateEntity();
			}
			else if (m_menuOptions[m_selectedIndex] == "Modify Entities")
			{
				ModifyEntity();
			}
			else if (m_menuOptions[m_selectedIndex] == "List Entities")
			{
				ListEntities();
			}
			else if (m_menuOptions[m_selectedIndex] == "Exit Debug Menu")
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

void DebugMenu::Render()
{
	int windowWidth = 0, windowHeight = 0;
	SDL_GetRendererOutputSize(m_renderer, &windowWidth, &windowHeight);

	SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 150);
	SDL_RenderClear(m_renderer);

	TTF_Font* font = TTF_OpenFont("Assets/arial.ttf", 24);

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

	SDL_RenderPresent(m_renderer);

	TTF_CloseFont(font);
}

void DebugMenu::CreateEntity()
{
	std::cout << "Creating new entity..." << std::endl;
	ScreenReader::Speak("Creating new entity.");

	//std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << "Entity enter name: ";
	ScreenReader::Speak("Enter the name of the entity.");
	std::string name;
	std::getline(std::cin, name);
	if (name.empty())
	{
		name = "Unnamed";
	}

	Entity& newEntity = g_manager.addEntity();
	newEntity.setName(name);
	ScreenReader::Speak("Entity created with name " + name);
	std::cout << "Entity \"" << name << "\" created." << std::endl;

	std::cout << "What group would you like to add this Entity to?" << std::endl;
	ScreenReader::Speak("What group would you like to add this Entity to?");

	//Currently hardcoding, adapt later 
	std::cout << "Available groups:" << std::endl;
	std::cout << "0: Enemies" << std::endl;
	std::cout << "1: Players" << std::endl;
	std::cout << "2: Maps" << std::endl;
	std::cout << "3: Colliders" << std::endl;
	std::cout << "4: Objects" << std::endl;
	std::cout << "5: Projectiles" << std::endl;

	ScreenReader::Speak("Available groups.");
	ScreenReader::Speak("Select 0 for Enemies.");
	ScreenReader::Speak("Select 1 for Players.");
	ScreenReader::Speak("Select 2 for Maps.");
	ScreenReader::Speak("Select 3 for Colliders.");
	ScreenReader::Speak("Select 4 for Objects.");
	ScreenReader::Speak("Select 5 for Projectiles.");

	int groupChoice;
	std::cin >> groupChoice;

	if (groupChoice < 0 || groupChoice > 5)
	{
		ScreenReader::Speak("Invalid group choice. Defaulting to Object");
		std::cout << "Invalid group choice. Defaulting to Object" << std::endl;
		groupChoice = 4; //defualt to object
		return;
	}

	newEntity.addGroup(groupChoice);


	//Prompt to add TransformComponent
	std::cout << "Would you like to add a TransformComponent? (1 for yes, 0 for no): ";
	ScreenReader::Speak("Would you like to add a TransformComponent? Press 1 for yes or 0 for no.");
	int addTransform = 0;
	std::cin >> addTransform;
	if (addTransform == 1)
	{
		float xPos = 0.0f, yPos = 0.0f, scale = 1.0f;
		int height = 128, width = 128;

		std::cout << "Enter X position (float): ";
		ScreenReader::Speak("Enter the X position as a float.");
		std::cin >> xPos;

		std::cout << "Enter Y position (float): ";
		ScreenReader::Speak("Enter the Y position as a float.");
		std::cin >> yPos;

		std::cout << "Enter height (int): ";
		ScreenReader::Speak("Enter the height as an integer.");
		std::cin >> height;

		std::cout << "Enter width (int): ";
		ScreenReader::Speak("Enter the width as an integer.");	
		std::cin >> width;

		std::cout << "Enter scale (float): ";
		ScreenReader::Speak("Enter the scale as a float.");
		std::cin >> scale;

		newEntity.addComponent<TransformComponent>(xPos, yPos, height, width, scale);
		ScreenReader::Speak("Transform component added.");
		std::cout << "Transform component added." << std::endl;

	}

	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	//Prompt to add SpriteComponent
	std::cout << "Would you like to add a SpriteComponent? (1 for yes, 0 for no): ";
	ScreenReader::Speak("Would you like to add a SpriteComponent? Press 1 for yes or 0 for no.");
	int addSprite = 0;
	std::cin >> addSprite;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	if (addSprite == 1)
	{
		std::string textureID;
		bool animated = false;

		std::cout << "Enter texture ID: " << std::endl;
		std::cout << "Alternatively, press enter to use the default texture ID." << std::endl;
		ScreenReader::Speak("Enter the texture ID.");
		ScreenReader::Speak("Alternatively, press enter to use the default texture ID.");
		std::getline(std::cin, textureID);
		if (textureID.empty())
		{
			textureID = "DefaultTexture";
		}

		std::cout << "Is the sprite animated? (1 for yes, 0 for no): ";
		ScreenReader::Speak("Is the sprite animated? Press 1 for yes or 0 for no.");
		std::cin >> animated;

		newEntity.addComponent<SpriteComponent>(textureID, animated);
		ScreenReader::Speak("Sprite component added.");
		std::cout << "Sprite component added." << std::endl;
	}

	g_manager.refresh();

	std::this_thread::sleep_for(std::chrono::seconds(2));
}

void DebugMenu::ModifyEntity()
{
	std::vector<Entity*> entities = g_manager.getEntities();

	if (entities.empty()) 
	{
		ScreenReader::Speak("No entities available to modify.");
		std::cout << "No entities available to modify." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return;
	}

	std::cout << "Available entities:" << std::endl;
	for (size_t i = 0; i < entities.size(); i++) {
		std::cout << i << ": " << entities[i]->getName() << std::endl;
	}

	std::cout << "Enter the index of the entity to modify: ";
	int entityIndex;
	std::cin >> entityIndex;

	if (entityIndex < 0 || entityIndex >= static_cast<int>(entities.size())) {
		ScreenReader::Speak("Invalid entity index.");
		std::cout << "Invalid entity index." << std::endl;
		return;
	}

	Entity* selectedEntity = entities[entityIndex];

	std::cout << "Modification Options:" << std::endl;
	std::cout << "1: Modify TransformComponent" << std::endl;
	std::cout << "2: Modify SpriteComponent" << std::endl;
	std::cout << "3: Add missing component" << std::endl;
	std::cout << "Enter your choice: ";

	int modChoice;
	std::cin >> modChoice;

	if (modChoice == 1)
	{
		if (selectedEntity->hasComponent<TransformComponent>())
		{
			TransformComponent& tc = selectedEntity->getComponent<TransformComponent>();
			std::stringstream ss;
			ss << "Current Position: (" << tc.m_position.m_x << ", " << tc.m_position.m_y << ")" << std::endl;
			std::string xyPosition = ss.str();
			std::cout << xyPosition << std::endl;

			std::stringstream widthSS;
			widthSS << "Current Width: " << tc.m_width << std::endl;
			std::string widthStr = widthSS.str();
			std::cout << widthStr;

			std::stringstream heightSS;
			heightSS << "Current Height: " << tc.m_height << std::endl;
			std::string heightStr = heightSS.str();
			std::cout << heightStr;

			std::stringstream scaleSS;
			scaleSS << "Current Scale: " << tc.m_scale << std::endl;
			std::string scaleStr = scaleSS.str();
			std::cout << scaleStr;

			ScreenReader::Speak(xyPosition);
			ScreenReader::Speak(widthStr);
			ScreenReader::Speak(heightStr);
			ScreenReader::Speak(scaleStr);


			std::cout << "Enter new X position (float): ";
			ScreenReader::Speak("Enter new X position as a float.");
			float newX;
			std::cin >> newX;

			std::cout << "Enter new Y position (float): ";
			ScreenReader::Speak("Enter new Y position as a float.");
			float newY;
			std::cin >> newY;

			std::cout << "Enter new width (int): ";
			ScreenReader::Speak("Enter new width as an integer.");
			int newWidth;
			std::cin >> newWidth;

			std::cout << "Enter new height (int): ";
			ScreenReader::Speak("Enter new height as an integer.");
			int newHeight;
			std::cin >> newHeight;

			std::cout << "Enter new scale (float): ";
			ScreenReader::Speak("Enter new scale as a float.");
			float newScale;
			std::cin >> newScale;

			tc.m_position.m_x = newX;
			tc.m_position.m_y = newY;
			tc.m_width = newWidth;
			tc.m_height = newHeight;
			tc.m_scale = newScale;
			
			ScreenReader::Speak("Transform component modified.");
			std::cout << "Transform component updated." << std::endl;
		}
		else
		{
			ScreenReader::Speak("Entity does not have a Transform component.");
			std::cout << "Entity does not have a Transform component." << std::endl;
		}
	}
	else if (modChoice == 2)
	{
		if (selectedEntity->hasComponent<SpriteComponent>())
		{
			SpriteComponent& sc = selectedEntity->getComponent<SpriteComponent>();

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

			std::cout << "Enter new texture ID: ";
			ScreenReader::Speak("Enter new texture ID.");
			std::string newTexture;
			std::cin >> newTexture;
			std::getline(std::cin, newTexture);
			if (newTexture.empty())
			{
				newTexture = "DefaultTexture";
			}
			sc.setTexture(newTexture);

			std::cout << "Is the sprite animated? (1 for yes, 0 for no): ";
			ScreenReader::Speak("Is the sprite animated? Press 1 for yes or 0 for no.");
			int isAnimated;
			std::cin >> isAnimated;
			if (isAnimated == 1)
			{
				sc.enableAnimation();
			}

			ScreenReader::Speak("Sprite component modified.");
			std::cout << "Sprite component updated." << std::endl;
		}
		else 
		{
			ScreenReader::Speak("Entity does not have a Sprite component.");
			std::cout << "Entity does not have a Sprite component." << std::endl;
		}
	}
	else if (modChoice == 3)
	{

		std::cout << "Select component to add:" << std::endl;
		std::cout << "1: TransformComponent" << std::endl;
		std::cout << "2: SpriteComponent" << std::endl;

		ScreenReader::Speak("Select component to add.");
		ScreenReader::Speak("1 TransformComponent.");
		ScreenReader::Speak("2 SpriteComponent.");

		int addChoice;
		std::cin >> addChoice;

		if (addChoice == 1) 
		{
			if (selectedEntity->hasComponent<TransformComponent>()) 
			{
				ScreenReader::Speak("Entity already has a Transform component.");
				std::cout << "Entity already has a Transform component." << std::endl;
			}
			else 
			{
				float xPos = 0.0f, yPos = 0.0f, scale = 1.0f;
				int height = 128, width = 128;

				std::cout << "Enter X position (float): ";
				ScreenReader::Speak("Enter the X position as a float.");
				std::cin >> xPos;

				std::cout << "Enter Y position (float): ";
				ScreenReader::Speak("Enter the Y position as a float.");
				std::cin >> yPos;

				std::cout << "Enter height (int): ";
				ScreenReader::Speak("Enter the height as an integer.");
				std::cin >> height;

				std::cout << "Enter width (int): ";
				ScreenReader::Speak("Enter the width as an integer.");
				std::cin >> width;

				std::cout << "Enter scale (float): ";
				ScreenReader::Speak("Enter the scale as a float.");
				std::cin >> scale;

				selectedEntity->addComponent<TransformComponent>(xPos, yPos, height, width, scale);
				ScreenReader::Speak("Transform component added.");
				std::cout << "Transform component added." << std::endl;
			}
		}
		else if (addChoice == 2) 
		{
			if (selectedEntity->hasComponent<SpriteComponent>()) 
			{
				ScreenReader::Speak("Entity already has a Sprite component.");
				std::cout << "Entity already has a Sprite component." << std::endl;
			}
			else 
			{
				std::string textureID;
				bool animated = false;

				std::cout << "Enter texture ID: ";
				std::cout << "Alternatively, press enter to use the default texture ID." << std::endl;
				ScreenReader::Speak("Enter the texture ID.");
				ScreenReader::Speak("Alternatively, press enter to use the default texture ID.");
				std::getline(std::cin, textureID);
				if (textureID.empty())
				{
					textureID = "DefaultTexture";
				}

				std::cout << "Is the sprite animated? (1 for yes, 0 for no): ";
				ScreenReader::Speak("Is the sprite animated? Press 1 for yes or 0 for no.");
				std::cin >> animated;

				selectedEntity->addComponent<SpriteComponent>(textureID, animated);
				ScreenReader::Speak("Sprite component added.");
				std::cout << "Sprite component added." << std::endl;
			}
		}
	}
	else 
	{
		ScreenReader::Speak("Invalid modification choice.");
		std::cout << "Invalid modification choice." << std::endl;
	}

	g_manager.refresh();
 }


void DebugMenu::ListEntities()
{
	std::vector<Entity*> entites = g_manager.getEntities();

	if (entites.empty())
	{
		ScreenReader::Speak("No entities are present.");
		std::cout << "No entities are present." << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(2));
		return;
	}

	std::stringstream ss;
	ss << "There are " << entites.size() << " entites.";
	std::string totalStr = ss.str();
	ScreenReader::Speak(totalStr);
	std::cout << totalStr << std::endl;

	for (size_t i = 0; i < entites.size(); i++)
	{
		std::stringstream entityInfo;
		std::string name = entites[i]->getName();
		if (name.empty() || name == "Unnamed")
		{
			entityInfo << "Entity " << i << " is " << (entites[i]->isActive() ? "active" : "inactive") << ".";
		}
		else
		{
			entityInfo << "Entity " << name << " is " << (entites[i]->isActive() ? "active" : "inactive") << ".";
		}
		std::string infoString = entityInfo.str();
		ScreenReader::Speak(infoString);
		std::cout << infoString << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	std::this_thread::sleep_for(std::chrono::seconds(2));
}

void DebugMenu::AddComponentToEntity()
{
	std::cout << "Enter entity index to add a TransformComponent: ";
	int entityIndex;
	std::cin >> entityIndex;

	ScreenReader::Speak("Transform component added to entity.");
	std::cout << "Transform component added to entity " << entityIndex << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(2));
}


