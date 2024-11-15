#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D m_position;
	Vector2D m_velocity;

	int m_speed = 3;

	TransformComponent()
	{
		m_position.m_x = 0.0f;
		m_position.m_y = 0.0f;
	}

	TransformComponent(float xPos, float yPos)
	{
		m_position.m_x = xPos;
		m_position.m_y = yPos;
	}

	void init() override
	{
		m_velocity.m_x = 0;
		m_velocity.m_y = 0;
	}

	void update() override
	{
		m_position.m_x += m_velocity.m_x * m_speed;
		m_position.m_y += m_velocity.m_y * m_speed;
	}
};