#pragma once
#include "ECS.h"

class PositionComponent : public Component
{
public:
	int getxPos() { return m_xPos; }
	int getyPos() { return m_yPos; }

	void init() override
	{
		m_xPos = 0;
		m_yPos = 0;
	}

	void update() override
	{
		m_xPos++;
		m_yPos++;
	}

	void setPosition(int xPos, int yPos)
	{
		m_xPos = xPos;
		m_yPos = yPos;
	}

private:
	int m_xPos;
	int m_yPos;
};