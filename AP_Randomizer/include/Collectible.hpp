#pragma once
#include "Unreal/UnrealCoreStructs.hpp"

namespace GameData {
	using RC::Unreal::FVector;

	class Collectible {
	public:
		Collectible(FVector new_position) {
			position = new_position;
			checked = false;
		}

		Collectible(FVector new_position, std::tuple<FVector, std::string, int> new_alternate_position) {
			position = new_position;
			alternate_position = new_alternate_position;
			checked = false;
		}

		Collectible(FVector new_position, std::vector<std::pair<std::string, int>> new_options) {
			position = new_position;
			required_options = new_options;
			checked = false;
		}

		Collectible(FVector new_position, std::vector<std::pair<std::string, int>> new_options, std::wstring new_time_trial_actor_name) {
			position = new_position;
			required_options = new_options;
			checked = false;
			time_trial_actor_name = new_time_trial_actor_name;
		}

		bool Check() {
			checked = true;
			return checked;
		}

		bool IsChecked() const {
			return checked;}

		FVector GetPosition(std::unordered_map<std::string, int> option_set) const {
			if (alternate_position) {
				const auto& [pos, option_name, option_value] = *alternate_position;
				if (option_set.at(option_name) == option_value) {
					return pos;
				}
			}
			return position;
		}

		bool CanCreate(std::unordered_map<std::string, int> option_set) const {
			for (const auto& [option_name, option_value] : required_options) {
				if (option_set.at(option_name) != option_value) {
					return false;
				}
			}
			// Note that this always returns true if the collectible has no required options.
			return true;
		}

		bool IsTimeTrial() const {
			return time_trial_actor_name.has_value();
		}

		bool HasTimeTrialActorName(std::wstring actor_name) const {
			if (!time_trial_actor_name) {
				return false;
			}

			return *time_trial_actor_name == actor_name;
		}

	private:
		FVector position;
		std::optional<std::tuple<FVector, std::string, int>> alternate_position;
		bool checked;
		std::vector<std::pair<std::string, int>> required_options;
		std::optional<std::wstring> time_trial_actor_name;
	};
}