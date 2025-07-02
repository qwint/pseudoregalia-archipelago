#pragma once
#include <mutex>
#include <queue>
#include <unordered_set>
#include "Unreal/TArray.hpp"
#include "Unreal/World.hpp"
#include "Engine.hpp"
#include "Logger.hpp"
#include "Client.hpp"

namespace Engine {
	using namespace RC::Unreal; // Give Engine easy access to Unreal objects
	using std::variant;
	using std::wstring;
	using std::to_wstring;
	using std::shared_ptr;
	using std::mutex;
	using std::lock_guard;
	using std::get;
	using std::unordered_set;
	using std::vector;
	using std::optional;
	using std::deque;
	using std::pair;

	// Private members
	namespace {
		void SyncMajorKeys();
		void SyncHealthPieces();
		void SyncSmallKeys();
		void SyncAbilities();
		void SpawnCollectible(int64_t, FVector);
		void AddMessages(UObject*);

		// keeps track of collectibles spawned since the last time SpawnCollectibles was called. this is necessary because
		// time trials may try to spawn their collectibles multiple times if the player beats the time trial more than once
		// without getting the collectible
		unordered_set<int64_t> spawned_collectibles;

		bool has_initialized_console = false;
		deque<pair<wstring, wstring>> messages;

		const wstring intro_markdown = L"<System>Welcome to Pseudoregalia Archipelago! You can connect by pressing Enter to open the console, then typing</>\n<System>/connect ip:port \"player name\"</>\n\n<System>Once you've obtained Solar Wind, you can toggle it with either Left Ctrl or the top face button on your controller.</>";
		const wstring intro_plain = L"Welcome to Pseudoregalia Archipelago! You can connect by pressing Enter to open the console, then typing\n/connect ip:port \"player name\"\n\nOnce you've obtained Solar Wind, you can toggle it with either Left Ctrl or the top face button on your controller.";

		struct BlueprintFunctionInfo {
			variant<wstring, UObject*> parent;
			wstring function_name;
			shared_ptr<void> params;
		};

		mutex blueprint_function_mutex;
		bool awaiting_item_sync;
		std::queue<BlueprintFunctionInfo> blueprint_function_queue;
	} // End private members


	// Returns the current map as a Map enum.
	GameData::Map Engine::GetCurrentMap() {
		static FName controller_name = FName(STR("PlayerController"));
		UObject* player_controller = UObjectGlobals::FindFirstOf(controller_name);
		wstring world_name = player_controller->GetWorld()->GetName();
		return GameData::MapNameToEnum(world_name);
	}

	// Queues up a blueprint function to be executed.
	void Engine::ExecuteBlueprintFunction(variant<wstring, UObject*> new_parent, wstring new_name, shared_ptr<void> params) {
		lock_guard<mutex> guard(blueprint_function_mutex);
		blueprint_function_queue.push(BlueprintFunctionInfo(new_parent, new_name, params));
	}

	// Runs once every engine tick.
	void Engine::OnTick(UObject* blueprint) {
		// Queue up item syncs together to avoid queueing a bajillion functions on connection or world release.
		if (awaiting_item_sync) {
			SyncHealthPieces();
			SyncSmallKeys();
			SyncMajorKeys();
			SyncAbilities();
			awaiting_item_sync = false;
		}

		// Engine tick runs in a separate thread from the client so it needs to be locked.
		lock_guard<mutex> guard(blueprint_function_mutex);
		while (!blueprint_function_queue.empty()) {
			BlueprintFunctionInfo info = blueprint_function_queue.front();
			UObject* object;
			if (std::holds_alternative<wstring>(info.parent)) {
				wstring parent_name = get<wstring>(info.parent);
				object = UObjectGlobals::FindFirstOf(parent_name);
				if (!object) {
					Log(L"Could not find blueprint with name " + parent_name, LogType::Error);
					blueprint_function_queue.pop();
					continue;
				}
			}
			else {
				object = get<UObject*>(info.parent);
				if (object->IsUnreachable()) {
					Log(L"Could not call " + info.function_name + L" because the blueprint was unreachable.", LogType::Error);
					blueprint_function_queue.pop();
					continue;
				}
			}

			UFunction* function = object->GetFunctionByName(info.function_name.c_str());
			if (!function) {
				Log(L"Could not find function " + info.function_name, LogType::Error);
				blueprint_function_queue.pop();
				continue;
			}
			Log(L"Executing " + info.function_name);
			// Need to cast to raw pointer to feed to ProcessEvent, but the memory will still be freed automatically
			void* ptr(info.params.get());
			object->ProcessEvent(function, ptr);
			blueprint_function_queue.pop();
		}
	}

	// Calls blueprint's AP_SpawnCollectible function for each unchecked collectible in a map.
	void Engine::SpawnCollectibles() {
		spawned_collectibles.clear();
		// This function must loop through instead of calling once with an array;
		// as of 10/11/23 the params struct method I use can't easily represent FVectors or FTransforms in C++.
		// This might be worked around by storing positions as three separate numbers instead and constructing the vectors in BP,
		// but I don't think it's worth changing right now since this is just called once each map load.
		std::unordered_map<int64_t, GameData::Collectible> collectible_map = GameData::GetCollectiblesOfZone(GetCurrentMap());
		for (const auto& [id, collectible] : collectible_map) {
			SpawnCollectible(id, collectible.GetPosition(GameData::GetOptions()));
		}
	}

	// Queues all item sync functions.
	void Engine::SyncItems() {
		awaiting_item_sync = true;
	}

	void Engine::ToggleSlideJump() {
		if (GameData::ToggleSlideJump()) {
			SyncAbilities();
		}
	}

	// Kills Sybil.
	void Engine::VaporizeGoat() {
		shared_ptr<void> dissolve_delay(new double(0));
		ExecuteBlueprintFunction(L"BP_PlayerGoatMain_C", L"BPI_CombatDeath", dissolve_delay);
	}

	void Engine::DespawnCollectible(const int64_t id) {
		std::vector<UObject*> collectibles{};
		UObjectGlobals::FindAllOf(STR("BP_APCollectible_C"), collectibles);
		for (auto const collectible : collectibles) {
			void* property_ptr = collectible->GetValuePtrByPropertyName(STR("id"));
			int64_t* new_id = static_cast<int64_t*>(property_ptr);
			if (*new_id == id) {
				Log(L"Manually despawning collectible with id " + to_wstring(id));
				ExecuteBlueprintFunction(collectible, L"Despawn", nullptr);
				break;
			}
			// It's fine if we don't find the collectible, it could just be in another map or already despawned
		}
	}

	void VerifyVersion() {
		// this implementation assumes players connect after loading into the game. if the connect flow ever changes,
		// this will need to be updated
		if (!GameData::CanHaveTimeTrial(GetCurrentMap())) {
			Log("Unable to verify game version.", LogType::Error);
			return;
		}

		int game_version = GameData::GetOptions().at("game_version");
		std::vector<UObject*> time_trials{};
		UObjectGlobals::FindAllOf(L"BP_TimeTrial_C", time_trials);
		bool time_trials_found = time_trials.size() != 0;
		if (game_version == GameData::MAP_PATCH && !time_trials_found) {
			Log("Game version map_patch was chosen in the player options, but it seems like you are playing on full gold. Switch to map patch for the intended experience.", LogType::Error);
		}
		else if (game_version == GameData::FULL_GOLD && time_trials_found) {
			Log("Game version full_gold was chosen in the player options, but it seems like you are playing on map patch. Switch to full gold for the intended experience.", LogType::Error);
		}
	}

	void SpawnTimeTrialCollectibleIfBeaten(UObject* obj) {
		wstring name = obj->GetName();
		optional<GameData::TimeTrial> time_trial = GameData::GetTimeTrial(GetCurrentMap(), name);
		if (!time_trial) {
			Log(L"Collectible not found for time trial " + name);
			return;
		}
		auto& [id, position] = *time_trial;
		int32_t medal_tier = *static_cast<int32_t*>(obj->GetValuePtrByPropertyName(L"medalTier"));
		if (medal_tier < 1) {
			Log(L"Time trial for collectible " + to_wstring(id) + L" has not been beaten");
			return;
		}
		SpawnCollectible(id, position);
	}

	void PrintToConsole(wstring markdown_text, wstring plain_text, optional<UObject*> console) {
		struct ConsoleLineInfo {
			FText markdown;
			FText plain;
		};
		FText ue_markdown(markdown_text);
		FText ue_plain(plain_text);
		std::shared_ptr<void> params(new ConsoleLineInfo{ ue_markdown, ue_plain });
		if (console) {
			ExecuteBlueprintFunction(*console, L"AP_PrintToConsole", params);
		}
		else {
			ExecuteBlueprintFunction(L"AP_DeluxeConsole_C", L"AP_PrintToConsole", params);
		}
	}

	void SaveMessage(wstring markdown, wstring plain) {
		if (messages.size() == 100) {
			// only maintain the last 100 messages
			messages.pop_front();
		}
		messages.push_back(pair<wstring, wstring>{ markdown, plain });
	}

	void InitializeConsole(UObject* console) {
		if (has_initialized_console) {
			AddMessages(console);
		}
		else if (GetCurrentMap() != GameData::Map::TitleScreen) {
			has_initialized_console = true;
			PrintToConsole(intro_markdown, intro_plain, console);
		}
	}

	void HealPlayer() {
		GameData::Map map = GetCurrentMap();
		if (map == GameData::Map::TitleScreen || map == GameData::Map::EndScreen) {
			// don't try to heal unless in a gameplay level
			return;
		}

		shared_ptr<void> Amount(new double(10));
		ExecuteBlueprintFunction(L"BP_PlayerGoatMain_C", L"healPlayer", Amount);
	}

	void GivePlayerPower() {
		GameData::Map map = GetCurrentMap();
		if (map == GameData::Map::TitleScreen || map == GameData::Map::EndScreen) {
			// don't try to give power unless in a gameplay level
			return;
		}

		struct ChangePowerAmountInfo {
			double A;
			bool forceUpdatePowerLevel;
		};
		shared_ptr<void> power_params(new ChangePowerAmountInfo{ 10, false });
		ExecuteBlueprintFunction(L"BP_PlayerGoatMain_C", L"changePowerAmount", power_params);
		ExecuteBlueprintFunction(L"BP_PlayerGoatMain_C", L"updatePlayerCurrentStatValues", nullptr);
	}


	// Private functions
	namespace {
		void SyncHealthPieces() {
			shared_ptr<void> hp_params(new int(GameData::GetHealthPieces()));
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetHealthPieces", hp_params);
		}

		void SyncSmallKeys() {
			shared_ptr<void> small_key_params(new int(GameData::GetSmallKeys()));
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetSmallKeys", small_key_params);
		}

		void SyncMajorKeys() {
			struct MajorKeyInfo {
				TArray<bool> keys;
			};
			TArray<bool> ue_keys;
			bool* major_keys = GameData::GetMajorKeys();
			for (int i = 0; i < 5; i++) {
				ue_keys.Add(major_keys[i]);
			}
			shared_ptr<void> major_key_params(new MajorKeyInfo{ ue_keys });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetMajorKeys", major_key_params);
		}

		void SyncAbilities() {
			struct AddUpgradeInfo {
				TArray<FName> names;
				TArray<int> counts;
				bool slidejump_disabled;
			};
			TArray<FName> ue_names;
			TArray<int> ue_counts;
			bool toggle = GameData::SlideJumpDisabled();

			for (const auto& [upgrade_name, upgrade_count] : GameData::GetUpgradeTable()) {
				FName new_name(upgrade_name);
				ue_names.Add(new_name);
				ue_counts.Add(upgrade_count);
			}
			shared_ptr<void> upgrade_params(new AddUpgradeInfo{ ue_names, ue_counts, toggle });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SetUpgrades", upgrade_params);
		}

		void SpawnCollectible(int64_t id, FVector position) {
			if (!Client::IsMissingLocation(id)) {
				Log(L"Collectible with id " + to_wstring(id) + L" was not spawned because it is not a missing location.");
				return;
			}
			if (spawned_collectibles.contains(id)) {
				Log(L"Collectible with id " + to_wstring(id) + L" has already been spawned");
				return;
			}
			Log(L"Spawning collectible with id " + to_wstring(id));
			struct CollectibleSpawnInfo {
				int64_t new_id;
				FVector new_position;
				int32_t classification;
			};
			shared_ptr<void> collectible_info(new CollectibleSpawnInfo{ id, position, GameData::GetClassification(id) });
			ExecuteBlueprintFunction(L"BP_APRandomizerInstance_C", L"AP_SpawnCollectible", collectible_info);
			spawned_collectibles.insert(id);
		}

		void AddMessages(UObject* console) {
			struct AddMessagesInfo {
				TArray<FText> markdown_messages;
				TArray<FText> plain_messages;
			};
			TArray<FText> ue_markdown_messages = { FText(intro_markdown) };
			TArray<FText> ue_plain_messages = { FText(intro_plain) };
			for (const auto& [markdown, plain] : messages) {
				ue_markdown_messages.Add(FText(markdown));
				ue_plain_messages.Add(FText(plain));
			}
			std::shared_ptr<void> params(new AddMessagesInfo{ ue_markdown_messages, ue_plain_messages });
			ExecuteBlueprintFunction(console, L"AP_AddMessages", params);
		}
	} // End private functions
}