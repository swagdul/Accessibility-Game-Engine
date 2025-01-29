#include "AccessibilityManager.h"
#include <iostream>

bool AccessibilityManager::m_highContrastEnabled = false;
int AccessibilityManager::m_fontSize = 16;
bool AccessibilityManager::m_keyboardNavigationEnabled = false;

void AccessibilityManager::EnableHighContrastMode(bool enable)
{
	m_highContrastEnabled = true;

	if (enable)
	{
		std::cout << "High Contrast Mode Enabled\n";
	}
	else
	{
		std::cout << "High Contrast Mode Disabled\n";
	}
}

bool AccessibilityManager::IsHighContrastEnabled()
{
	return m_highContrastEnabled;
}

int AccessibilityManager::GetFontSize()
{
	return m_fontSize;
}
