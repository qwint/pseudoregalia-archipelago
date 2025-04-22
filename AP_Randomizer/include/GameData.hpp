#pragma once
#include <map>
#include <vector>
#include "Collectible.hpp"

namespace GameData {
	enum class ItemType {
		MajorAbility,
		MinorAbility,
		HealthPiece,
		SmallKey,
		MajorKey,
		Unknown
	};

	enum class Map {
		TitleScreen,
		Dungeon,
		Castle,
		Keep,
		Library,
		Theatre,
		Bailey,
		Underbelly,
		Tower,
		Chambers,
		EndScreen,
	};

	// this is an enum instead of an enum class because we cast to an int when
	// calling the spawn collectible blueprint
	enum Classification {
		Generic,
		GenericProgression,
		GenericUsefulOrTrap,
		GenericFiller,
		MajorAbility,
		MinorAbility,
		HealthPiece,
		MajorKey,
		SmallKey,
	};

	void Initialize();
	void Close();
	int GetHealthPieces();
	int GetSmallKeys();
	bool* GetMajorKeys();
	void SetOption(std::string, int);
	std::unordered_map<std::string, int> GetOptions();
	std::unordered_map<std::wstring, int> GetUpgradeTable();
	std::unordered_map<int64_t, Collectible> GetCollectiblesOfZone(Map);
	std::list<int64_t> GetLocations();
	void SetPseudoItemClassification(int64_t, int64_t);
	void SetOffWorldItemClassification(int64_t, Classification);
	Classification GetClassification(int64_t);
	void CheckLocation(const int64_t);
	ItemType ReceiveItem(int64_t);
	Map MapNameToEnum(std::wstring);
	bool ToggleSlideJump();
	bool SlideJumpDisabled();
}