#pragma once
#include "Unreal/UnrealCoreStructs.hpp"

namespace GameData {
	using RC::Unreal::FVector;

	class Collectible {
	public:
		Collectible(FVector new_position) {
			position = new_position;
		}

		FVector GetPosition() const {
			return position;
		}

	private:
		FVector position;
	};
}