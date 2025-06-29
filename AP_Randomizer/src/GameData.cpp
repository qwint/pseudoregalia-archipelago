#pragma once
#include "GameData.hpp"
#include "Logger.hpp"
#include "Settings.hpp"
#include "Client.hpp"

namespace GameData {
    using std::unordered_map;
    using std::list;
    using std::wstring;
    using std::string;
    using std::optional;
    using std::pair;
    using std::tuple;

    // Private members
    namespace {
        ItemType GetItemType(int64_t);

        int health_pieces;
        int small_keys;
        bool major_keys[5];
        unordered_map<wstring, int> upgrade_table;
        unordered_map<Map, unordered_map<int64_t, Collectible>> collectible_table;
        unordered_map<Map, unordered_map<wstring, TimeTrial>> time_trial_table;
        unordered_map<int64_t, Classification> lookup_location_id_to_classification;
        unordered_map<string, int> options;
        bool slidejump_owned;
        bool slidejump_disabled;

        const unordered_map<wstring, Map> map_names = {
            {L"TitleScreen",            Map::TitleScreen},
            {L"ZONE_Dungeon",           Map::Dungeon},
            {L"ZONE_LowerCastle",       Map::Castle},
            {L"Zone_Upper",             Map::Keep},
            {L"Zone_Library",           Map::Library},
            {L"Zone_Theatre",           Map::Theatre},
            {L"ZONE_Exterior",          Map::Bailey},
            {L"Zone_Caves",             Map::Underbelly},
            {L"Zone_Tower",             Map::Tower},
            {L"Zone_PrincessChambers",  Map::Chambers},
            {L"EndScreen",              Map::EndScreen},
        };

        // The two lookup tables below could be combined into one table with something like an ItemIdInfo struct,
        // and should be if a third table would be added.
        const unordered_map<int64_t, ItemType> lookup_item_id_to_type = {
            {2365810001, ItemType::MajorAbility},
            {2365810002, ItemType::MajorAbility},
            {2365810003, ItemType::MajorAbility},
            {2365810004, ItemType::MajorAbility},
            {2365810005, ItemType::MajorAbility},
            {2365810006, ItemType::MajorAbility},
            {2365810007, ItemType::MajorAbility},
            {2365810008, ItemType::MajorAbility},
            {2365810009, ItemType::MajorAbility},
            {2365810010, ItemType::MajorAbility},
            {2365810011, ItemType::MinorAbility},
            {2365810012, ItemType::MinorAbility},
            {2365810013, ItemType::MinorAbility},
            {2365810014, ItemType::MinorAbility},
            {2365810015, ItemType::MinorAbility},
            {2365810016, ItemType::MinorAbility},
            {2365810017, ItemType::MinorAbility},
            {2365810018, ItemType::MinorAbility},

            {2365810026, ItemType::MajorAbility},
            {2365810027, ItemType::MinorAbility},
            {2365810028, ItemType::MajorAbility},

            {2365810021, ItemType::MajorKey},
            {2365810022, ItemType::MajorKey},
            {2365810023, ItemType::MajorKey},
            {2365810024, ItemType::MajorKey},
            {2365810025, ItemType::MajorKey},

            {2365810019, ItemType::HealthPiece},
            {2365810020, ItemType::SmallKey},

            {2365810029, ItemType::MinorAbility},
            {2365810030, ItemType::MinorAbility},
            {2365810031, ItemType::MinorAbility},
            {2365810032, ItemType::MinorAbility},
            {2365810033, ItemType::MinorAbility},
            {2365810034, ItemType::MinorAbility},
            {2365810035, ItemType::MinorAbility},

            {2365810036, ItemType::MinorAbility},
        };

        const unordered_map<int64_t, wstring> lookup_item_id_to_upgrade = {
            {2365810001, L"attack"},
            {2365810002, L"powerBoost"},
            {2365810003, L"airKick"},
            {2365810004, L"slide"},
            {2365810005, L"SlideJump"},
            {2365810006, L"plunge"},
            {2365810007, L"chargeAttack"},
            {2365810008, L"wallRide"},
            {2365810009, L"Light"},
            {2365810010, L"projectile"},
            {2365810011, L"extraKick"},
            {2365810012, L"airRecovery"},
            {2365810013, L"mobileHeal"},
            {2365810014, L"magicHaste"},
            {2365810015, L"healBoost"},
            {2365810016, L"damageBoost"},
            {2365810017, L"magicPiece"},
            {2365810018, L"outfitPro"},
            {2365810026, L"progressiveSlide"},
            {2365810027, L"extraKick"}, // Used for split kicks, just treats them like heliacal
            {2365810028, L"progressiveBreaker"},
            {2365810029, L"outfitFaith"},
            {2365810030, L"outfitShoujo"},
            {2365810031, L"outfitSweater"},
            {2365810032, L"outfitClassy"},
            {2365810033, L"outfitKnight"},
            {2365810034, L"outfitJam"},
            {2365810035, L"outfitPast"},
            {2365810036, L"map"},
        };

        // This is for location ids, not items, so it can't be combined with the tables above.
        const unordered_map<int64_t, Map> lookup_location_id_to_zone = {
            {2365810001, Map::Dungeon},
            {2365810002, Map::Dungeon},
            {2365810003, Map::Dungeon},
            {2365810004, Map::Dungeon},
            {2365810005, Map::Dungeon},
            {2365810006, Map::Dungeon},
            {2365810007, Map::Dungeon},
            {2365810054, Map::Dungeon},
            {2365810008, Map::Castle},
            {2365810009, Map::Castle},
            {2365810010, Map::Castle},
            {2365810011, Map::Castle},
            {2365810012, Map::Castle},
            {2365810013, Map::Castle},
            {2365810014, Map::Castle},
            {2365810015, Map::Castle},
            {2365810016, Map::Castle},
            {2365810017, Map::Castle},
            {2365810018, Map::Castle},
            {2365810019, Map::Castle},
            {2365810055, Map::Castle},
            {2365810062, Map::Castle},
            {2365810020, Map::Keep},
            {2365810021, Map::Keep},
            {2365810022, Map::Keep},
            {2365810023, Map::Keep},
            {2365810024, Map::Keep},
            {2365810025, Map::Keep},
            {2365810056, Map::Keep},
            {2365810026, Map::Library},
            {2365810027, Map::Library},
            {2365810028, Map::Library},
            {2365810029, Map::Library},
            {2365810051, Map::Library},
            {2365810052, Map::Library},
            {2365810053, Map::Library},
            {2365810057, Map::Library},
            {2365810030, Map::Theatre},
            {2365810031, Map::Theatre},
            {2365810032, Map::Theatre},
            {2365810033, Map::Theatre},
            {2365810034, Map::Theatre},
            {2365810035, Map::Theatre},
            {2365810058, Map::Theatre},
            {2365810036, Map::Bailey},
            {2365810037, Map::Bailey},
            {2365810038, Map::Bailey},
            {2365810039, Map::Bailey},
            {2365810040, Map::Bailey},
            {2365810059, Map::Bailey},
            {2365810041, Map::Underbelly},
            {2365810042, Map::Underbelly},
            {2365810043, Map::Underbelly},
            {2365810044, Map::Underbelly},
            {2365810045, Map::Underbelly},
            {2365810046, Map::Underbelly},
            {2365810047, Map::Underbelly},
            {2365810048, Map::Underbelly},
            {2365810060, Map::Underbelly},
            {2365810049, Map::Tower},
            {2365810050, Map::Tower},
            {2365810061, Map::Tower},
        };
    } // End private members


    int GameData::GetHealthPieces() {
        return health_pieces;
    }

    int GameData::GetSmallKeys() {
        return small_keys;
    }

    bool* GameData::GetMajorKeys() {
        return major_keys;
    }

    unordered_map<wstring, int> GameData::GetUpgradeTable() {
        return upgrade_table;
    }

    void GameData::SetOption(string option_name, int value) {
        Log("Set option " + option_name + " to " + std::to_string(value));
        options[option_name] = value;
    }

    unordered_map<string, int> GameData::GetOptions() {
        return options;
    }

    unordered_map<int64_t, Collectible> GameData::GetCollectiblesOfZone(Map current_map) {
        return collectible_table[current_map];
    }

    list<int64_t> GameData::GetMissingSpawnableLocations() {
        list<int64_t> location_ids;
        for (const auto& [_, zone_map] : collectible_table) {
            for (const auto& [location_id, _] : zone_map) {
                if (Client::IsMissingLocation(location_id)) {
                    location_ids.push_back(location_id);
                }
            }
        }
        for (const auto& [_, zone_map] : time_trial_table) {
            for (const auto& [_, time_trial] : zone_map) {
                const int64_t location_id = time_trial.first;
                if (Client::IsMissingLocation(location_id)) {
                    location_ids.push_back(location_id);
                }
            }
        }
        return location_ids;
    }

    void GameData::SetPseudoItemClassification(int64_t location_id, int64_t item_id) {
        if (Settings::GetItemDisplay() == Settings::ItemDisplay::GenericAll) {
            return;
        }

        ItemType type = lookup_item_id_to_type.at(item_id);
        switch (type) {
        case ItemType::MajorAbility:
            lookup_location_id_to_classification[location_id] = Classification::MajorAbility;
            break;
        case ItemType::MinorAbility:
            lookup_location_id_to_classification[location_id] = Classification::MinorAbility;
            break;
        case ItemType::HealthPiece:
            lookup_location_id_to_classification[location_id] = Classification::HealthPiece;
            break;
        case ItemType::MajorKey:
            lookup_location_id_to_classification[location_id] = Classification::MajorKey;
            break;
        case ItemType::SmallKey:
            lookup_location_id_to_classification[location_id] = Classification::SmallKey;
            break;
        }
    }

    void GameData::SetOffWorldItemClassification(int64_t location_id, Classification classification) {
        if (Settings::GetItemDisplay() == Settings::ItemDisplay::GenericNonPseudo ||
            Settings::GetItemDisplay() == Settings::ItemDisplay::GenericAll) {
            return;
        }

        lookup_location_id_to_classification[location_id] = classification;
    }

    Classification GameData::GetClassification(int64_t location_id) {
        if (lookup_location_id_to_classification.contains(location_id)) {
            return lookup_location_id_to_classification[location_id];
        }
        else {
            return Classification::Generic;
        }
    }

    void GameData::Initialize() {
        using std::vector;
        using std::pair;

        collectible_table = {
            {Map::Dungeon, unordered_map<int64_t, Collectible>{
            // Dream Breaker
                {2365810001, Collectible(FVector(-3500.0, 4950.0, -50.0))},
            // Slide
                {2365810002, Collectible(FVector(16650, 2600, 2350))},
            // Alcove Near Mirror
                {2365810003, Collectible(FVector(1150, -400, 1050))},
            // Dark Orbs
                {2365810004, Collectible(FVector(18250, -9750, 4200))},
            // Past Poles
                {2365810005, Collectible(FVector(6800, 8850, 3850))},
            // Rafters
                {2365810006, Collectible(FVector(7487, 1407, 4250))},
            // Strong Eyes
                {2365810007, Collectible(FVector(750, 8850, 2650))},
                    }},
            {Map::Castle, unordered_map<int64_t, Collectible> {
            // Indignation
                {2365810008, Collectible(FVector(5400, 2100, -550))},
            // Alcove Near Dungeon
                {2365810009, Collectible(FVector(1600, 8000, -1400))},
            // Balcony
                {2365810010, Collectible(FVector(16400, 3800, 1200))},
            // Corner Corridor
                {2365810011, Collectible(FVector(11850, 1000, -300))},
            // Floater In Courtyard
                {2365810012, Collectible(FVector(-5000, -600, 2050))},
            // Locked Door
                {2365810013, Collectible(FVector(2700, -1700, -500))},
            // Platform In Main Halls
                {2365810014, Collectible(FVector(7950, 2750, -200))},
            // Tall Room Near Wheel Crawlers
                {2365810015, Collectible(FVector(-4100, -8200, 2950))},
            // Wheel Crawlers
                {2365810016, Collectible(FVector(-10050, -3700, 1000))},
            // High Climb From Courtyard
                {2365810017, Collectible(FVector(-3150, 11500, 6300))},
            // Alcove Near Scythe Corridor
                {2365810018, Collectible(FVector(-9600, 21750, 5400))},
            // Near Theatre Front
                {2365810019, Collectible(FVector(3390, 21150, 6600))},
            // Memento
                {2365810062, Collectible(FVector(8950, 6450, -175))},
                    }},
            {Map::Keep, unordered_map<int64_t, Collectible> {
            // Strikebreak
                {2365810020, Collectible(FVector(10050, 1800, 1000))},
            // Alcove Near Locked Door
                {2365810021, Collectible(FVector(800, 2500, 1200))},
            // Levers Room
                {2365810022, Collectible(FVector(1050, 15700, 1300))},
            // Lonely Throne
                {2365810023, Collectible(FVector(14350, -50, 1350))},
            // Near Theatre
                {2365810024, Collectible(FVector(-3900, -6109, -450))},
            // Sunsetter
                {2365810025, Collectible(FVector(-3000, 4900, -400))},
                    }},
            {Map::Library, unordered_map<int64_t, Collectible> {
            // Sun Greaves
                {2365810026, Collectible(FVector(-4150, 9200, -100))},
            // Upper Back
                {2365810027, Collectible(FVector(-9250, -1850, 1250))},
            // Locked Door Across
                {2365810028, Collectible(FVector(-1300, -6750, -700))},
            // Locked Door Left
                {2365810029, Collectible(FVector(-3750, -4170, -700))},
            // Split Greaves 1
                {2365810051, Collectible(FVector(-4150, 9160, 0))},
            // Split Greaves 2
                {2365810052, Collectible(FVector(-4100, 9250, -100))},
            // Split Greaves 3
                {2365810053, Collectible(FVector(-4200, 9250, -100))},
                    }},
            {Map::Theatre, unordered_map<int64_t, Collectible> {
            // Soul Cutter
                {2365810030, Collectible(FVector(8500, 7850, -1400))},
            // Back Of Auditorium
                {2365810031, Collectible(FVector(-1600, 1500, 2600))},
            // Center Stage
                {2365810032, Collectible(FVector(5200, 1550, 700))},
            // Locked Door
                {2365810033, Collectible(FVector(-1460, -2550, 2240))},
            // Murderous Goat
                {2365810034, Collectible(FVector(255, 1150, 50))},
            // Corner Beam
                {2365810035, Collectible(FVector(-14100, -150, 1950))},
                    }},
            {Map::Bailey, unordered_map<int64_t, Collectible> {
            // Solar Wind
                {2365810036, Collectible(FVector(-1100, 10850, 150))},
            // Center Steeple
                {2365810037, Collectible(FVector(2350, 7260, 2110))},
            // Cheese Bell
                {2365810038, Collectible(FVector(5040, 7150, 2500))},
            // Guarded Hand
                {2365810039, Collectible(FVector(-1787, 5236, 650))},
            // Inside Building
                {2365810040, Collectible(FVector(3007, 3457, 300))},
                    }},
            {Map::Underbelly, unordered_map<int64_t, Collectible> {
            // Ascendant Light
                {2365810041, Collectible(FVector(-5400, 6650, 6750))},
            // Alcove Near Light
                {2365810042, Collectible(FVector(-2550, 12300, 4400))},
            // Building Near Little Guy
                {2365810043, Collectible(FVector(-4350, 28350, 1850))},
            // Locked Door
                {2365810044, Collectible(FVector(18896, 7937, 1200))},
            // Main Room
                {2365810045, Collectible(FVector(-726, 19782, 3200))},
            // Rafters Near Keep
                {2365810046, Collectible(FVector(19600, 17750, 5700))},
            // Strikebreak Wall
                {2365810047, Collectible(FVector(11300, 12700, 3107))},
            // Surrounded By Holes
                {2365810048, Collectible(FVector(33050, 24100, 3850), tuple<FVector, string, int>{FVector(31900, 26250, 3850), "game_version", FULL_GOLD})},
                    }},
            {Map::Tower, unordered_map<int64_t, Collectible> {
            // Cling Gem
                {2365810049, Collectible(FVector(13350, 5250, 4150))},
            // Atop The Tower
                {2365810050, Collectible(FVector(9650, 5250, 7100))},
                    }},
        };

        // map -> time trial actor name -> location id + position
        time_trial_table = {
            {Map::Dungeon, {
                {L"BP_TimeTrial_C_1", {2365810054, FVector(-3350, -4300, 850)}},
            }},
            {Map::Castle, {
                {L"BP_TimeTrial_C_1", {2365810055, FVector(3200, -1700, -500)}},
            }},
            {Map::Keep, {
                {L"BP_TimeTrial_C_3", {2365810056, FVector(14350, 400, 1250)}},
            }},
            {Map::Library, {
                {L"BP_TimeTrial_C_2", {2365810057, FVector(-2850, 3600, 900)}},
            }},
            {Map::Theatre, {
                {L"BP_TimeTrial_C_1", {2365810058, FVector(-14750, 3900, 100)}},
            }},
            {Map::Bailey, {
                {L"BP_TimeTrial_C_1", {2365810059, FVector(1150, 5250, -600)}},
            }},
            {Map::Underbelly, {
                {L"BP_TimeTrial_C_1", {2365810060, FVector(1250, 18000, 3000)}},
            }},
            {Map::Tower, {
                {L"BP_TimeTrial_C_3", {2365810061, FVector(10750, 3050, 4000)}},
            }},
        };


        upgrade_table = {
            {L"attack", 0},
            {L"powerBoost", 0},
            {L"airKick", 0},
            {L"slide", 0},
            {L"SlideJump", 0},
            {L"plunge", 0},
            {L"chargeAttack", 0},
            {L"wallRide", 0},
            {L"Light", 0},
            {L"projectile", 0},
            {L"extraKick", 0},
            {L"airRecovery", 0},
            {L"mobileHeal", 0},
            {L"magicHaste", 0},
            {L"healBoost", 0},
            {L"damageBoost", 0},
            {L"magicPiece", 0},
            {L"outfitPro", 0},
            {L"outfitFaith", 0},
            {L"outfitShoujo", 0},
            {L"outfitSweater", 0},
            {L"outfitClassy", 0},
            {L"outfitKnight", 0},
            {L"outfitJam", 0},
            {L"outfitPast", 0},
            {L"map", 0},
        };

        slidejump_owned = false;
        slidejump_disabled = false;
        small_keys = 0;
        for (bool &k : major_keys) {
            k = false;
        }
    }

    void GameData::Close() {
        collectible_table = {};
        time_trial_table = {};
        slidejump_owned = false;
        slidejump_disabled = false;
        small_keys = 0;
        for (bool &k : major_keys) {
            k = false;
        }
        upgrade_table = {
            {L"attack", 0},
            {L"powerBoost", 0},
            {L"airKick", 0},
            {L"slide", 0},
            {L"SlideJump", 0},
            {L"plunge", 0},
            {L"chargeAttack", 0},
            {L"wallRide", 0},
            {L"Light", 0},
            {L"projectile", 0},
            {L"extraKick", 0},
            {L"airRecovery", 0},
            {L"mobileHeal", 0},
            {L"magicHaste", 0},
            {L"healBoost", 0},
            {L"damageBoost", 0},
            {L"magicPiece", 0},
            {L"outfitPro", 0},
            {L"outfitFaith", 0},
            {L"outfitShoujo", 0},
            {L"outfitSweater", 0},
            {L"outfitClassy", 0},
            {L"outfitKnight", 0},
            {L"outfitJam", 0},
            {L"outfitPast", 0},
            {L"map", 0},
        };
    }

    ItemType GameData::ReceiveItem(int64_t id) {
        ItemType type = lookup_item_id_to_type.at(id);
        switch (type) {
        case ItemType::MajorAbility:
        case ItemType::MinorAbility:
            upgrade_table[lookup_item_id_to_upgrade.at(id)]++;
            if (!slidejump_owned) {
                if (upgrade_table[L"slide"] && upgrade_table[L"SlideJump"]
                    || upgrade_table[L"progressiveSlide"] >= 2) {
                    slidejump_owned = true;
                }
            }
            break;
        case ItemType::HealthPiece:
            health_pieces++;
            break;
        case ItemType::SmallKey:
            small_keys++;
            break;
        case ItemType::MajorKey:
            // Remove prefix digits from id to assign directly to major_keys array
            major_keys[id - 2365810021] = true;
            break;
        default:
            Log(L"You were sent an item, but its id wasn't recognized. Verify that you're playing on the same version this seed was generated on.");
            break;
        }
        return type;
    }

    Map GameData::MapNameToEnum(wstring map_name) {
        return map_names.at(map_name);
    }

    bool GameData::ToggleSlideJump() {
        if (!slidejump_owned) {
            Log(L"Slidejump is not obtained");
            return false;
        }

        slidejump_disabled = !slidejump_disabled;
        if (slidejump_disabled) {
            Log(L"Solar wind is now OFF.");
        }
        else {
            Log(L"Solar wind is now ON.");
        }
        return true;
    }

    bool GameData::SlideJumpDisabled() {
        return slidejump_disabled;
    }

    bool CanHaveTimeTrial(Map map) {
        switch (map) {
        case Map::Dungeon:
        case Map::Castle:
        case Map::Keep:
        case Map::Library:
        case Map::Theatre:
        case Map::Bailey:
        case Map::Underbelly:
        case Map::Tower:
            return true;
        }
        return false;
    }

    optional<TimeTrial> GetTimeTrial(Map map, wstring time_trial_actor_name) {
        if (!time_trial_table.contains(map)) {
            return {};
        }
        if (!time_trial_table.at(map).contains(time_trial_actor_name)) {
            return {};
        }
        return time_trial_table.at(map).at(time_trial_actor_name);
    }
}