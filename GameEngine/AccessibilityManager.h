#pragma once
class AccessibilityManager
{
public:

	static void EnableHighContrastMode(bool enable);
	static void AdjustFontSize(int size);
	static void EnableKEyboardNavigation(bool enable);

	static bool IsHighContrastEnabled();

private:

	static bool m_highContrastEnabled;
	static int m_fontSize;
	static bool m_keyboardNavigationEnabled;
};

