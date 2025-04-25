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
		const string settings_filename = "Mods/AP_Randomizer/settings.toml";
		ItemDisplay item_display = ItemDisplay::Full;
	}

	void Load() {
		ifstream settings_file(settings_filename);
		if (!settings_file.good()) {
			Log("Settings file not found");
			return;
		}

		try {
			toml::table settings_table = toml::parse(settings_file);

			optional<string> item_display_setting = settings_table["settings"]["item_display"].value<string>();
			if (item_display_setting) {
				bool set = false;
				if (item_display_setting.value() == "generic_non_pseudo") {
					item_display = ItemDisplay::GenericNonPseudo;
					set = true;
				}
				else if (item_display_setting.value() == "generic_all") {
					item_display = ItemDisplay::GenericAll;
					set = true;
				}
				if (set) {
					Log("item_display set to " + item_display_setting.value());
				}
			}

			Log("Loaded settings");
		}
		catch (const toml::parse_error& err) {
			Log("Failed to parse settings: " + string(err.description()));
		}
	}

	ItemDisplay GetItemDisplay() {
		return item_display;
	}
}
