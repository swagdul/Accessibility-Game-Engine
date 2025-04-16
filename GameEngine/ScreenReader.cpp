#include "ScreenReader.h"
#include <iostream>
#include <string>

//Windows
#ifdef _WIN32
#include <Windows.h>
#include <sapi.h>
#pragma warning(push)
#pragma warning(disable:4996)
#include <sphelper.h>
#pragma warning(pop)


static std::wstring StringToWString(const std::string& string)
{
	if (string.empty()) return std::wstring();
	int sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, string.c_str(), static_cast<int>(string.size()), NULL, 0);
	std::wstring wstr(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, string.c_str(), static_cast<int>(string.size()), &wstr[0], sizeNeeded);
	return wstr;
}

void ScreenReader::Speak(const std::wstring& text)
{
	HRESULT hr = ::CoInitialize(NULL);
	if (FAILED(hr))
	{
		std::wcerr << L"ScreenReader: Failed to initialize COM library." << std::endl;
		return;
	}

	ISpVoice* pVoice = nullptr;
	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, reinterpret_cast<void**>(&pVoice));
	if (SUCCEEDED(hr) && pVoice != nullptr)
	{
		pVoice->SetRate(3);

		hr = pVoice->Speak(text.c_str(), SPF_DEFAULT, NULL);
		if (FAILED(hr)) {
			std::wcerr << L"ScreenReader: Failed to speak text." << std::endl;
		}
		pVoice->Release();
	}
	else
	{
		std::wcerr << L"ScreenReader: Failed to create SAPI voice instance." << std::endl;
	}

	CoUninitialize();
}

void ScreenReader::Speak(const std::string& text)
{
	Speak(StringToWString(text));
}

#else
//macOS//Linux
#include <cstdlib>

void ScreenReader::Speak(const std::wstring& text)
{
	std::string utf8Text(text.begin(), text.end());
	Speak(utf8Text);
}

void ScreenReader::Speak(const std::string& text)
{
	std::string command = "espeak \"" + text + "\"";
	int result = system(command.c_str());

	if (result != 0)
	{
		std::cerr << "ScreenReader: Failed to execute espeak command." << std::endl;
	}
}

#endif