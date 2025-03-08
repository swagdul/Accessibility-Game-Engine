#pragma once

struct Animation
{
	int m_index;
	int m_frames;
	int m_speed;

	Animation()
	{
	}

	Animation(int index, int frames, int speed)
	{
		m_index = index;
		m_frames = frames;
		m_speed = speed;
	}
};
