#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D m_position;
	Vector2D m_velocity;

	int m_height = 128;
	int m_width = 128;
	int m_scale = 1;

	int m_speed = 3;

	TransformComponent()
	{
		m_position.m_x = 0.0f;
		m_position.m_y = 0.0f;
	}

	TransformComponent(float xPos, float yPos, int scale)
	{
		m_position.m_x = xPos;
		m_position.m_y = yPos;
		m_scale = scale;
	}

	TransformComponent(int scale)
	{
		m_position.m_x = 0.0f;
		m_position.m_y = 0.0f;
		m_scale = scale;
	}

	TransformComponent(float xPos, float yPos, int height, int width, int scale)
	{
		m_position.m_x = xPos;
		m_position.m_y = yPos;
		m_height = height;
		m_width = width;
		m_scale = scale;
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