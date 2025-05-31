#pragma once
#include "Unreal/UnrealCoreStructs.hpp"

namespace GameData {
	using RC::Unreal::FVector;

	class Collectible {
	public:
		Collectible(FVector new_position) {
			position = new_position;
		}

		Collectible(FVector new_position, std::tuple<FVector, std::string, int> new_alternate_position) {
			position = new_position;
			alternate_position = new_alternate_position;
		}

		Collectible(FVector new_position, std::wstring new_time_trial_actor_name) {
			position = new_position;
			time_trial_actor_name = new_time_trial_actor_name;
		}

		FVector GetPosition(std::unordered_map<std::string, int> option_set) const {
			if (alternate_position) {
				const auto& [pos, option_name, option_value] = *alternate_position;
				if (option_set.at(option_name) == option_value) {
					return pos;
				}
			}
			return position;
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
		std::optional<std::wstring> time_trial_actor_name;
	};
}