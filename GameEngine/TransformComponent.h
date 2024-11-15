#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D m_position;

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

	void update() override
	{

	}
};