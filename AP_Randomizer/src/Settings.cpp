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
	using std::pair;
	using std::list;

	namespace {
		// if you run from the executable directory
		const string settings_filename1 = "Mods/AP_Randomizer/settings.toml";
		// if you run from the game directory
		const string settings_filename2 = "pseudoregalia/Binaries/Win64/Mods/AP_Randomizer/settings.toml";

		ItemDisplay item_display = ItemDisplay::Full;
		PopupsInitialState popups_initial_state = PopupsInitialState::ShowWithSound;
		bool popups_simplify_item_font = false;
		bool death_link = false;

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
			Log("Using default settings");
			return;
		}

		Log("Loading settings");
		item_display = ParseSetting(
			settings_table,
			"settings.item_display",
			unordered_map<string, ItemDisplay>{
				{ "full", ItemDisplay::Full },
				{ "generic_non_pseudo", ItemDisplay::GenericNonPseudo },
				{ "generic_all", ItemDisplay::GenericAll },
			},
			"full");
		death_link = ParseSetting(settings_table, "settings.death_link", false);
		popups_initial_state = ParseSetting(
			settings_table,
			"settings.popups.initial_state",
			unordered_map<string, PopupsInitialState>{
				{ "show_with_sound", PopupsInitialState::ShowWithSound },
				{ "show_muted", PopupsInitialState::ShowMuted },
				{ "hide", PopupsInitialState::Hide },
		},
			"show_with_sound");
		popups_simplify_item_font = ParseSetting(settings_table, "settings.popups.simplify_item_font", false);
	}

	ItemDisplay GetItemDisplay() {
		return item_display;
	}

	bool GetDeathLink() {
		return death_link;
	}

	PopupsInitialState GetPopupsInitialState() {
		return popups_initial_state;
	}

	bool GetPopupsSimplifyItemFont() {
		return popups_simplify_item_font;
	}

	namespace {
		template<class E> E ParseSetting(toml::table settings_table, string setting_path, unordered_map<string, E> option_map, string default_option) {
			optional<string> option = settings_table.at_path(setting_path).value<string>();
			if (option) {
				if (option_map.contains(*option)) {
					Log(setting_path + " = " + *option);
					return option_map.at(*option);
				}

				Log(setting_path + " = " + default_option + " (using default, unknown option " + *option + ")");
				return option_map.at(default_option);
			}

			Log(setting_path + " = " + default_option + " (using default, setting missing or not a string)");
			return option_map.at(default_option);
		}

		bool ParseSetting(toml::table settings_table, string setting_path, bool default_option) {
			optional<bool> option = settings_table.at_path(setting_path).value<bool>();
			if (option) {
				string option_string = *option ? "true" : "false";
				Log(setting_path + " = " + option_string);
				return *option;
			}

			string default_option_string = default_option ? "true" : "false";
			Log(setting_path + " = " + default_option_string + " (using default, setting missing or not a bool)");
			return default_option;
		}
	}
}
