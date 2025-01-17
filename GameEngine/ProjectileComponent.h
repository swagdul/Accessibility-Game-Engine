#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"

class ProjectileCompoment : public Component
{
public:

	ProjectileCompoment(int range, int speed, Vector2D velocity) : m_range(range), m_speed(speed), m_velocity(velocity)
	{

	}

	~ProjectileCompoment()
	{

	}

	void init() override
	{
		m_transform = &m_entity->getComponent<TransformComponent>();
		m_transform->m_velocity = m_velocity;
	}

	void update() override
	{
		m_distance += m_speed;

		if (m_distance > m_range)
		{
			m_entity->destroy();
		}
		else if (m_transform->m_position.m_x > Game::m_camera.x + Game::m_camera.w ||
				m_transform->m_position.m_x < Game::m_camera.x ||
				m_transform->m_position.m_y > Game::m_camera.y + Game::m_camera.h ||
				m_transform->m_position.m_y < Game::m_camera.y)
		{
			m_entity->destroy();
		}
	}

private:

	TransformComponent* m_transform;

	int m_range = 0;
	int m_speed = 0;
	int m_distance = 0;
	Vector2D m_velocity;
};