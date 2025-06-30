#pragma once

#include <iostream>
#include <fstream>
#include "Logger.hpp"
#include "Settings.hpp"
#include "toml++/toml.hpp"

namespace Settings {
	using std::optional;
	using std::string;
	using std::ifstream;

	namespace {
		// if you run from the executable directory
		const string settings_filename1 = "Mods/AP_Randomizer/settings.toml";
		// if you run from the game directory
		const string settings_filename2 = "pseudoregalia/Binaries/Win64/Mods/AP_Randomizer/settings.toml";

		ItemDisplay item_display = ItemDisplay::Full;
		Popups popups = Popups::ShowWithSound;
		bool death_link = false;
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

		try {
			toml::table settings_table = toml::parse(settings_file);
			Log("Loading settings");

			// item_display
			optional<string> item_display_setting = settings_table["settings"]["item_display"].value<string>();
			if (item_display_setting) {
				string setting_value = item_display_setting.value();
				if (setting_value == "full") {
					item_display = ItemDisplay::Full;
					Log("item_display set to " + setting_value);
				}
				else if (setting_value == "generic_non_pseudo") {
					item_display = ItemDisplay::GenericNonPseudo;
					Log("item_display set to " + setting_value);
				}
				else if (setting_value == "generic_all") {
					item_display = ItemDisplay::GenericAll;
					Log("item_display set to " + setting_value);
				}
				else {
					Log("Unknown option " + setting_value + " for item_display, using default option full");
				}
			}
			else {
				Log("Using default option full for item_display");
			}

			// popups
			optional<string> popups_setting = settings_table["settings"]["popups"].value<string>();
			if (popups_setting) {
				string setting_value = popups_setting.value();
				if (setting_value == "show_with_sound") {
					popups = Popups::ShowWithSound;
					Log("popups set to " + setting_value);
				}
				else if (setting_value == "show_muted") {
					popups = Popups::ShowMuted;
					Log("popups set to " + setting_value);
				}
				else if (setting_value == "hide") {
					popups = Popups::Hide;
					Log("popups set to " + setting_value);
				}
				else {
					Log("Unknown option " + setting_value + " for popups, using default option show_with_sound");
				}
			}
			else {
				Log("Using default option show_with_sound for popups");
			}

			// death_link
			optional<bool> death_link_setting = settings_table["settings"]["death_link"].value<bool>();
			if (death_link_setting) {
				death_link = death_link_setting.value();
				string setting_string = death_link_setting.value() ? "true" : "false";
				Log("death_link set to " + setting_string);
			}
			else {
				Log("Using default option false for death_link");
			}
		}
		catch (const toml::parse_error& err) {
			Log("Failed to parse settings: " + string(err.description()));
		}
	}

	ItemDisplay GetItemDisplay() {
		return item_display;
	}

	Popups GetPopups() {
		return popups;
	}

	bool GetDeathLink() {
		return death_link;
	}
}
