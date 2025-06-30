#pragma once
#include "Unreal/FText.hpp"
#include "DynamicOutput/DynamicOutput.hpp"
#include "Logger.hpp"
#include "Engine.hpp"
#include "Timer.hpp"
#include "Settings.hpp"
#include "StringOps.hpp"

namespace Logger {
	using namespace RC::Output;
	using namespace RC::LogLevel;
	using RC::Unreal::FText;
	using std::wstring;
	using std::string;
	using std::list;
	using std::variant;
	using std::holds_alternative;
	using std::get;

	// Private members
	namespace {
		void PrintToPlayer(variant<wstring, ItemPopup>);

		list<variant<wstring, ItemPopup>> message_queue;
		bool popups_locked;
		// Just over 3 seconds is long enough to ensure only 2 popups can be on screen at once
		const float popup_delay_seconds(3.2f);
		bool messages_hidden;
		bool messages_muted;
	} // End private members

	void ShowPopup(string text) {
		ShowPopup(StringOps::ToWide(text));
	}

	void ShowPopup(wstring text) {
		send<LogLevel::Verbose>(L"[APRandomizer] Message: " + text + L"\n");
		PrintToPlayer(text);
	}

	void ShowPopup(ItemPopup popup) {
		send<LogLevel::Verbose>(L"[APRandomizer] Message: " + popup.preamble + L" " + popup.item + L" " + popup.info + L"\n");
		PrintToPlayer(popup);
	}

	void Logger::Log(string text, LogType type) {
		Log(StringOps::ToWide(text), type);
	}

	void Logger::Log(wstring text, LogType type) {
		switch (type) {
		case LogType::Console: {
			send<LogLevel::Default>(L"[APRandomizer] Console: " + text + L"\n");
			break;
		}

		case LogType::System: {
			send<LogLevel::Verbose>(L"[APRandomizer] System: " + text + L"\n");
			wstring markdown_text = L"<System>" + text + L"</>";
			PrintToConsole(markdown_text, text);
			break;
		}

		case LogType::Warning: {
			send<LogLevel::Warning>(L"[APRandomizer] WARNING: " + text + L"\n");
			break;
		}

		case LogType::Error: {
			send<LogLevel::Error>(L"[APRandomizer] ERROR: " + text + L"\n");
			wstring markdown_text = L"<Error>Error: " + text + L"</>";
			PrintToConsole(markdown_text, text);
			break;
		}

		default: {
			send<LogLevel::Default>(L"[APRandomizer] : " + text + L"\n");
			break;
		}
		} // End switch
	}

	void Logger::PrintToConsole(std::wstring markdown_text, std::wstring plain_text) {
		Engine::SaveMessage(markdown_text, plain_text);
		Engine::PrintToConsole(markdown_text, plain_text);
	}

	void Logger::PrintToConsole(std::wstring text) {
		PrintToConsole(text, text);
	}

	void Logger::OnTick() {
		// This implementation is slightly awkward but the whole UI is gonna get refactored eventually anyway so whatever.
		if (popups_locked) {
			return;
		}

		if (!message_queue.empty()) {
			variant<wstring, ItemPopup> front = message_queue.front();
			message_queue.pop_front();
			if (holds_alternative<wstring>(front)) {
				struct PrintToPlayerInfo {
					FText text;
					bool mute_sound;
				};
				FText new_text(get<wstring>(front));
				std::shared_ptr<void> params(new PrintToPlayerInfo{ new_text, messages_muted });
				Engine::ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_PrintMessage", params);
			}
			else {
				struct PrintItemToPlayerInfo {
					FText preamble;
					FText item;
					FText info;
					bool mute_sound;
					bool simplify_item_popup_font;
				};
				ItemPopup popup = get<ItemPopup>(front);
				std::shared_ptr<void> params(new PrintItemToPlayerInfo{
					FText(popup.preamble),
					FText(popup.item),
					FText(popup.info),
					messages_muted,
					Settings::GetSimplifyItemPopupFont(),
				});
				Engine::ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_PrintItemMessage", params);
			}
			Timer::RunTimerInGame(popup_delay_seconds, &popups_locked);
		}
	}

	void Logger::ToggleMessageMute() {
		messages_muted = !messages_muted;
		if (messages_muted) {
			Log(L"Popup sounds are now muted.", LogType::System);
		}
		else {
			Log(L"Popup sounds are no longer muted.", LogType::System);
		}
	}

	void Logger::ToggleMessageHide() {
		messages_hidden = !messages_hidden;
		if (messages_hidden) {
			message_queue.clear();
			Log(L"Popups are now hidden.", LogType::System);
		}
		else {
			Log(L"Popups are no longer hidden.", LogType::System);
		}
	}

	void Logger::Init() {
		switch (Settings::GetPopups()) {
		case Settings::Popups::ShowMuted:
			messages_muted = true;
			break;
		case Settings::Popups::Hide:
			messages_hidden = true;
			break;
		}
	}


	// Private functions
	namespace {
		void PrintToPlayer(variant<wstring, ItemPopup> message) {
			if (!messages_hidden) {
				message_queue.push_back(message);
			}
		}
	} // End private functions
}