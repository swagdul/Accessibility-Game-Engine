#include "SaveLoad.h"
#include "ECS.h"
#include "Game.h"
#include "Components.h"
#include "AppearanceSettings.h"
#include <fstream>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

extern Manager g_manager;

json SerialiseEntity(Entity* entity)
{
	json j;
	j["name"] = entity->getName();

	if (entity->hasComponent<TransformComponent>())
	{
		auto& tc = entity->getComponent<TransformComponent>();

		j["transform"] =
		{
			{"x", tc.m_position.m_x},
			{"y", tc.m_position.m_y},
			{"width", tc.m_width},
			{"height", tc.m_height},
			{"scale", tc.m_scale}
		};
	}

	if (entity->hasComponent<SpriteComponent>())
	{
		auto& sc = entity->getComponent<SpriteComponent>();

		j["sprite"] =
		{
			{"textureID", sc.getTextureID()},
			{"animated", sc.isAnimated()},
			{"frames", sc.getAnimatiomFrames()},
			{"speed", sc.getAnimationSpeed()},
			{"flipped", sc.isFlipped()}
		};
	}

	if (entity->hasComponent<ColliderComponent>())
	{
		auto& cc = entity->getComponent<ColliderComponent>();

		j["collider"] =
		{
			{"tag", cc.m_tag},
		};
	}

	if (entity->hasComponent<HealthComponent>())
	{
		auto& hc = entity->getComponent<HealthComponent>();

		j["health"] =
		{
			{"current", hc.getCurrentHealth()},
			{"max", hc.getMaxHealth()},
			{"player", hc.isPlayer()}
		};
	}

	json groups = json::array();
	constexpr std::size_t maxGroups = 32;
	for (size_t i = 0; i < maxGroups; i++)
	{
		if (entity->hasGroup(i))
		{
			groups.push_back(i);
		}
	}
	j["groups"] = groups;

	return j;
}

void SaveEntities(const std::vector<Entity*>& entities, const std::string& filename)
{
	json jEntities = json::array();

	for (auto& entity : entities)
	{
		if (entity->isDebugCreated())
		{
			jEntities.push_back(SerialiseEntity(entity));
		}
	}

	json appearance;
	SDL_Color normal = AppearanceSettings::Get().normalColour;
	SDL_Color highlight = AppearanceSettings::Get().highlightColour;
	int fontSize = AppearanceSettings::Get().fontSize;

	appearance["normal"] = { {"r", normal.r}, {"g", normal.g}, {"b", normal.b}, {"a", normal.a} };
	appearance["highlight"] = { {"r", highlight.r}, {"g", highlight.g}, {"b", highlight.b}, {"a", highlight.a} };
	appearance["fontSize"] = fontSize;

	json jSave;
	jSave["entities"] = jEntities;
	jSave["appearance"] = appearance;

	std::ofstream ofs(filename);
	if (!ofs.is_open())
	{
		std::cerr << "Failed to open file for writing: " << filename << std::endl;
		return;
	}

	ofs << jSave.dump(4);
	ofs.close();
	std::cout << "Entities saved to: " << filename << std::endl;
}

void DeserialiseEntity(const json& j)
{
	Entity& e = g_manager.addEntity();
	std::string name = j.value("name", "Unnamed");
	e.setName(name);

	e.setDebugCreated(true);

	if (j.contains("transform"))
	{
		json jTransform = j["transform"];
		float x = jTransform.value("x", 0.0f);
		float y = jTransform.value("y", 0.0f);
		int width = jTransform.value("width", 128);
		int height = jTransform.value("height", 128);
		float scale = jTransform.value("scale", 1.0f);
		e.addComponent<TransformComponent>(x, y, height, width, scale);
	}

	if (j.contains("sprite"))
	{
		json jSprite = j["sprite"];
		std::string textureID = jSprite.value("textureID", "DefaultTexture");
		bool animated = jSprite.value("animated", false);
		int frames = jSprite.value("frames", 7); 
		int speed = jSprite.value("speed", 100);     
		bool flipped = jSprite.value("flipped", false);
		e.addComponent<SpriteComponent>(textureID, animated, frames, speed, flipped);
	}

	if (j.contains("health"))
	{
		json jHealth = j["health"];
		int current = jHealth.value("current", 100);
		int max = jHealth.value("max", 100);
		bool isPlayer = jHealth.value("isPlayer", false);
		e.addComponent<HealthComponent>(max, isPlayer);
		auto& hc = e.getComponent<HealthComponent>();	
		hc.setCurrentHealth(current);
	}

	if (j.contains("collider"))
	{
		json jCollider = j["collider"];
		std::string tag = jCollider.value("tag", "");
		e.addComponent<ColliderComponent>(tag);
	}

	if (j.contains("groups"))
	{
		for (auto& group : j["groups"])
		{
			int groupIndex = group.get<int>();
			e.addGroup(groupIndex);
		}
	}
}

void LoadEntities(const std::string& filename)
{
	std::ifstream ifs(filename);

	if (!ifs.is_open())
	{
		std::cerr << "Failed to open file for reading: " << filename << std::endl;
		return;
	}

	json jSave;
	ifs >> jSave;
	ifs.close();


	if (jSave.contains("entities"))
	{
		for (auto& j : jSave["entities"])
		{
			DeserialiseEntity(j);
		}
	}

	if (jSave.contains("appearance")) 
	{
		json app = jSave["appearance"];

		if (app.contains("normal")) 
		{
			AppearanceSettings::Get().normalColour.r = app["normal"].value("r", 255);
			AppearanceSettings::Get().normalColour.g = app["normal"].value("g", 255);
			AppearanceSettings::Get().normalColour.b = app["normal"].value("b", 255);
			AppearanceSettings::Get().normalColour.a = app["normal"].value("a", 255);
		}

		if (app.contains("highlight")) 
		{
			AppearanceSettings::Get().highlightColour.r = app["highlight"].value("r", 255);
			AppearanceSettings::Get().highlightColour.g = app["highlight"].value("g", 0);
			AppearanceSettings::Get().highlightColour.b = app["highlight"].value("b", 0);
			AppearanceSettings::Get().highlightColour.a = app["highlight"].value("a", 255);
		}

		AppearanceSettings::Get().fontSize = app.value("fontSize", 24);

	}

	std::cout << "Entities  and appearance settings loaded from: " << filename << std::endl;
}

