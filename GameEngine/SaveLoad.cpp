#include "SaveLoad.h"
#include "ECS.h"
#include "Game.h"
#include "Components.h"
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
			{"animated", sc.isAnimated()}
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

	return j;
}

void SaveEntities(const std::vector<Entity*>& entities, const std::string& filename)
{
	json jEntities = json::array();

	for (auto& entity : entities)
	{
		jEntities.push_back(SerialiseEntity(entity));
	}

	std::ofstream ofs(filename);
	if (!ofs.is_open())
	{
		std::cerr << "Failed to open file for writing: " << filename << std::endl;
		return;
	}	

	ofs << jEntities.dump(4);
	ofs.close();
	std::cout << "Entities saved to: " << filename << std::endl;
}

void DeserialiseEntity(const json& j)
{
	Entity& e = g_manager.addEntity();
	std::string name = j.value("name", "Unnamed");
	e.setName(name);

	if (j.contains("transform"))
	{
		json jTransform = j["transform"];
		float x = jTransform.value("x", 0.0f);
		float y = jTransform.value("y", 0.0f);
		int width = jTransform.value("width", 128);
		int height = jTransform.value("height", 128);
		float scale = jTransform.value("scale", 1.0f);
		e.addComponent<TransformComponent>(x, y, width, height, scale);
	}

	if (j.contains("sprite"))
	{
		json jSprite = j["sprite"];
		std::string textureID = jSprite.value("textureID", "DefaultTexture");
		bool animated = jSprite.value("animated", false);
		e.addComponent<SpriteComponent>(textureID, animated);
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
}

void LoadEntities(const std::string& filename)
{
	std::ifstream ifs(filename);

	if (!ifs.is_open())
	{
		std::cerr << "Failed to open file for reading: " << filename << std::endl;
		return;
	}

	json jEntities;
	ifs >> jEntities;
	ifs.close();

	for (auto& j : jEntities)
	{
		DeserialiseEntity(j);
	}	

	std::cout << "Entities loaded from: " << filename << std::endl;
}

