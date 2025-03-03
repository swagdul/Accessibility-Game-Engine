#pragma once
#include <string>

class ScreenReader
{
public:
	static void Speak(const std::wstring& text);

	static void Speak(const std::string& text);
};

