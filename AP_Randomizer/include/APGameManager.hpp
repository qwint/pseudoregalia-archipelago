#pragma once
#include "DynamicOutput/DynamicOutput.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/UFunction.hpp"
#include "Unreal/World.hpp"
#include "APClient.hpp"
#include "APCollectible.hpp"

using namespace RC;
using namespace RC::Unreal;

namespace Pseudoregalia_AP {
	class APGameManager {
	public:
		static UWorld* GetWorld();
		static void OnBeginPlay(AActor*);
		static void PreProcessEvent(UObject*, UFunction*, void*);
		static void QueueItemUpdate();
		
	private:
		static bool hooked_into_returncheck;
		static bool item_update_pending;
		static void OnMapLoad(AActor*, UWorld*);
		static void OnReturnCheck(Unreal::UnrealScriptFunctionCallableContext&, void*);
		static void RegisterReturnCheckHook(AActor*);
		static void EmptyFunction(Unreal::UnrealScriptFunctionCallableContext&, void*);
		static void SyncItems(UObject*, UFunction*);
	};
}