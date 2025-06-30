#pragma once

#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Logger.hpp"
#include "Settings.hpp"
#include "toml++/toml.hpp"

namespace Settings {
	using std::optional;
	using std::string;
	using std::ifstream;
	using std::unordered_map;

	namespace {
		// if you run from the executable directory
		const string settings_filename1 = "Mods/AP_Randomizer/settings.toml";
		// if you run from the game directory
		const string settings_filename2 = "pseudoregalia/Binaries/Win64/Mods/AP_Randomizer/settings.toml";

		ItemDisplay item_display;
		Popups popups;
		bool simplify_item_popup_font;
		bool death_link;

		template<class E> E ParseSetting(toml::table, string, unordered_map<string, E>, string);
		bool ParseSetting(toml::table, string, bool);
	}

	void Load() {
		ifstream settings_file(settings_filename1);
		if (!settings_file.good()) {
			settings_file = ifstream(settings_filename2);
			if (!settings_file.good()) {
				Log("Settings file not found, using default settings");
				return;
			}
		}

		toml::table settings_table;
		try {
			settings_table = toml::parse(settings_file);
		}
		catch (const toml::parse_error& err) {
			Log("Failed to parse settings: " + string(err.description()));
			return;
		}

		Log("Loading settings");
		item_display = ParseSetting(
			settings_table,
			"item_display",
			unordered_map<string, ItemDisplay>{
				{ "full", ItemDisplay::Full },
				{ "generic_non_pseudo", ItemDisplay::GenericNonPseudo },
				{ "generic_all", ItemDisplay::GenericAll },
			},
			"full");
		popups = ParseSetting(
			settings_table,
			"popups",
			unordered_map<string, Popups>{
				{ "show_with_sound", Popups::ShowWithSound },
				{ "show_muted", Popups::ShowMuted },
				{ "hide", Popups::Hide },
			},
			"show_with_sound");
		simplify_item_popup_font = ParseSetting(settings_table, "simplify_item_popup_font", false);
		death_link = ParseSetting(settings_table, "death_link", false);
	}

	ItemDisplay GetItemDisplay() {
		return item_display;
	}

	Popups GetPopups() {
		return popups;
	}

	bool GetSimplifyItemPopupFont() {
		return simplify_item_popup_font;
	}

	bool GetDeathLink() {
		return death_link;
	}

	namespace {
		template<class E> E ParseSetting(toml::table settings_table, string setting_name, unordered_map<string, E> option_map, string default_option) {
			optional<string> setting = settings_table["settings"][setting_name].value<string>();
			if (setting) {
				string setting_value = setting.value();
				if (option_map.contains(setting_value)) {
					Log(setting_name + " set to " + setting_value);
					return option_map.at(setting_value);
				}

				Log("Unknown option " + setting_value + " for " + setting_name + ", using default option " + default_option);
				return option_map.at(default_option);
			}

			Log("Using default option " + default_option + " for " + setting_name);
			return option_map.at(default_option);
		}

		bool ParseSetting(toml::table settings_table, string setting_name, bool default_option) {
			optional<bool> setting = settings_table["settings"][setting_name].value<bool>();
			if (setting) {
				string setting_string = setting.value() ? "true" : "false";
				Log(setting_name + " set to " + setting_string);
				return setting.value();
			}

			string default_value_string = default_option ? "true" : "false";
			Log("Using default option " + default_value_string + " for " + setting_name);
			return default_option;
		}
	}
}
