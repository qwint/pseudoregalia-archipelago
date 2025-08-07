#pragma once
#include <variant>
#include "Unreal/UObject.hpp"
#include "GameData.hpp"

namespace Engine {
	using RC::Unreal::UObject;

	void ExecuteBlueprintFunction(std::variant<std::wstring, UObject*>, std::wstring, std::shared_ptr<void>);
	void OnTick(UObject*);
	void SyncItems();
	void SpawnCollectibles();
	void DespawnCollectible(const int64_t);
	void DespawnInteractable(const int64_t);
	GameData::Map GetCurrentMap();
	void ToggleSlideJump();
	void VaporizeGoat();
	void VerifyVersion();
	void SpawnTimeTrialCollectibleIfBeaten(UObject*);
	void PrintToConsole(std::wstring, std::wstring, std::optional<UObject*> = {});
	void SaveMessage(std::wstring, std::wstring);
	void InitializeConsole(UObject*);
	void HealPlayer();
	void GivePlayerPower();
	void WarpToSpawn();
	void SetTombstoneText(UObject*);
	void CreateMajorKeyHints(UObject*);
}