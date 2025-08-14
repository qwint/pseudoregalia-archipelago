#pragma once
#include <string>

namespace Logger {
	enum class LogType {
		Default,
		Console,
		System,
		Warning,
		Error
	};

	struct ItemPopup {
		std::wstring preamble;
		std::wstring item;
		std::wstring info;
	};

	void ShowPopup(std::string);
	void ShowPopup(std::wstring);
	void ShowPopup(ItemPopup);
	void Log(std::wstring, LogType = LogType::Default);
	void Log(std::string, LogType = LogType::Default);
	void OnTick();
	void ToggleMessageMute();
	void ToggleMessageHide();
	void Init();
}

// Don't want to have to prefix Logger twice every time we log something
using Logger::Log;
using Logger::LogType;