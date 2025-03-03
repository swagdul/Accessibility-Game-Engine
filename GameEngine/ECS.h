#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>
#include <string>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

using GroupBitSet = std::bitset<maxGroups>;

class Component
{
public:
	Entity* m_entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
public:
	Entity(Manager& manager) : m_manager(manager), m_name("Unnamed") {}

	void update()
	{
		for (auto& c : m_components) c->update();
	}

	void draw() 
	{
		for (auto& c : m_components) c->draw();
	}

	bool isActive() const { return m_active; }
	void destroy() { m_active = false; }

	bool hasGroup(Group group)
	{
		return m_groupBitSet[group];
	}

	void addGroup(Group group);

	void deleteGroup(Group group)
	{
		m_groupBitSet[group] = false;
	}

	template <typename T> bool hasComponent() const
	{
		return m_componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs> 
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->m_entity = this;
		std::unique_ptr<Component> uPtr{ c };
		m_components.emplace_back(std::move(uPtr));

		m_componentArray[getComponentTypeID<T>()] = c;
		m_componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template <typename T> T& getComponent() const
	{
		auto ptr(m_componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

	void setName(const std::string& name)
	{
		m_name = name;
	}

	const std::string& getName() const
	{
		return m_name;
	}

private:
	Manager& m_manager;

	bool m_active = true;
	std::vector<std::unique_ptr<Component>> m_components;

	ComponentArray m_componentArray;
	ComponentBitSet m_componentBitSet;

	GroupBitSet m_groupBitSet;

	std::string m_name;
};

class Manager
{
public:
	void update()
	{
		for (auto& e : m_entites) e->update();
	}

	void draw()
	{
		for (auto& e : m_entites) e->draw();
	}

	void refresh()
	{
		for (auto entityItem = 0u; entityItem < maxGroups; entityItem++)
		{
			auto& vector = m_groupedEntities[entityItem];
			vector.erase(
				std::remove_if(std::begin(vector), std::end(vector),
					[entityItem](Entity* entity)
			{
				return !entity->isActive() || !entity->hasGroup(entityItem);
			}), std::end(vector));

		}

		m_entites.erase(std::remove_if(std::begin(m_entites), std::end(m_entites),
			[](const std::unique_ptr<Entity>& mEntity)
			{
				return !mEntity->isActive();
			}),
			std::end(m_entites));
	}

	void AddToGroup(Entity* entity, Group group)
	{
		m_groupedEntities[group].emplace_back(entity);
	}

	std::vector<Entity*>& getGroup(Group group)
	{
		return m_groupedEntities[group];
	}

	Entity& addEntity()
	{
		Entity* e = new Entity(*this);
		std::unique_ptr<Entity> uPtr{ e };
		m_entites.emplace_back(std::move(uPtr));
		return *e;
	}

	std::vector<Entity*> getEntities() const
	{
		std::vector<Entity*> result;
		for (const auto& entity : m_entites)
		{
			result.push_back(entity.get());
		}
		return result;
	}

private:
	std::vector<std::unique_ptr<Entity>> m_entites;
	std::array<std::vector<Entity*>, maxGroups> m_groupedEntities;
};


