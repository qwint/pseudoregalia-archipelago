#pragma once
#pragma comment (lib, "crypt32") // Include crypt32.lib
#pragma warning(disable: 4267) // Disable conversion warnings from websocketpp and apclientpp
#pragma warning(disable: 4996) // Disable deprecated warnings from old asio version and apclientpp
#pragma warning(disable: 26495) // Disable uninitialized warnings from asio, websocketpp, and apclientpp
#pragma warning(disable: 26439) // Disable noexcept warnings from asio

// Boost is included, but not defining asio standalone results in a ton of errors in match_flags.hpp and wswrap_websocketpp.hpp.
#define ASIO_STANDALONE
#define BOOST_ALL_NO_LIB
#define APCLIENT_DEBUG
#include "apclient.hpp"
#include "apuuid.hpp"
#include "Engine.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include "Timer.hpp"
#include "DeathLinkMessages.hpp"
#include "StringOps.hpp"
#include "Settings.hpp"

namespace Client {
    using std::string;
    using std::wstring;
    using std::list;
    using std::optional;
    using std::vector;
    using std::unordered_map;
    using std::pair;

    namespace Hashes {
        using StringOps::HashNstring;
        constexpr size_t player_id = HashNstring("player_id");
        constexpr size_t item_id = HashNstring("item_id");
        constexpr size_t location_id = HashNstring("location_id");
    }

    // Private members
    namespace {
        typedef nlohmann::json json;
        typedef APClient::State ConnectionStatus;
        void ReceiveItems(const list<APClient::NetworkItem>&);
        string ProcessMessageText(const APClient::PrintJSONArgs&);
        optional<Logger::ItemPopup> BuildItemPopup(const APClient::PrintJSONArgs&);
        void ReceiveDeathLink(const json&);
        void ReceiveItemOnce(const APClient::PrintJSONArgs&);
        void Despawn(int64_t);
        void ParseKeyHints(const json&);
        vector<pair<wstring, wstring>> GetConsoleHintText(int64_t, int, list<int64_t>);

        // I don't think a mutex is required here because apclientpp locks the instance during poll().
        // If people report random crashes, especially when disconnecting, I'll revisit it.
        APClient* ap;
        const string game_name("Pseudoregalia");
        const string uuid(ap_get_uuid("Mods/AP_Randomizer/dlls/uuid"));
        const string cert_store("Mods/AP_Randomizer/dlls/cacert.pem");
        const int max_connection_retries = 3;
        int connection_retries = 0;
        bool death_link_locked;
        const float death_link_timer_seconds(4.0f);
    } // End private members

    void Client::Connect(const string uri, const string slot_name, const string password) {
        // Nuke any existing client in case uri needs to change.
        if (ap != nullptr) {
            delete ap;
        }
        GameData::Initialize();
        ap = new APClient(uuid, game_name, uri, cert_store);
        connection_retries = 0;
        string connect_message(
            "Attempting to connect to " + uri
            + " with name " + slot_name + "...");
        Log(connect_message, LogType::System);

        // The Great Wall Of Callbacks
        {
            // Executes when the server sends room info; attempts to connect the player.
            ap->set_room_info_handler([slot_name, password]() {
                Log("Received room info");
                int items_handling = 0b111;
                list<string> tags;
                if (Settings::GetDeathLink()) {
                    tags.push_back("DeathLink");
                }
                APClient::Version version{ 0, 7, 0 };
                ap->ConnectSlot(slot_name, password, items_handling, tags, version);
                });

            // Executes on successful connection to slot.
            ap->set_slot_connected_handler([](const json& slot_data) {
                Log("Connected to slot");
                for (json::const_iterator iter = slot_data.begin(); iter != slot_data.end(); iter++) {
                    string key = iter.key();
                    if (key == "key_hints") {
                        ParseKeyHints(iter.value());
                    }
                    else {
                        GameData::SetOption(key, iter.value());
                    }
                }
                SetZoneData();
                ap->LocationScouts(GameData::GetMissingSpawnableLocations());
                // Delay spawning collectibles so that we have time to receive checked locations and scouts.
                Timer::RunTimerRealTime(std::chrono::milliseconds(500), Engine::SpawnCollectibles);
                // Delay verifying version so that it shows up as the last message after connecting
                Timer::RunTimerRealTime(std::chrono::milliseconds(500), Engine::VerifyVersion);
                connection_retries = 0;
                });

            // Executes whenever a socket error is detected.
            // We want to only print an error after exactly X attempts.
            ap->set_socket_error_handler([](const string& error) {
                Log("Socket error: " + error);
                if (connection_retries == max_connection_retries) {
                    if (ap->get_player_number() >= 0) { // Seed is already in progress
                        Log(L"Lost connection with the server. Attempting to reconnect...", LogType::System);
                    }
                    else { // Attempting to connect to a new room
                        Log(L"Could not connect to the server. Please double-check the address and ensure the server is active.", LogType::System);
                    }
                }
                connection_retries++;
                });

            // Executes when the server refuses slot connection.
            ap->set_slot_refused_handler([](const list<string>& reasons) {
                string advice;
                if (std::find(reasons.begin(), reasons.end(), "InvalidSlot") != reasons.end()
                    || std::find(reasons.begin(), reasons.end(), "InvalidPassword") != reasons.end()) {
                    advice = "Please double-check your slot name and password.";
                }
                // Intentionally overwriting advice because slot name doesn't matter if the version is wrong.
                if (std::find(reasons.begin(), reasons.end(), "IncompatibleVersion") != reasons.end()) {
                    advice = "Please double-check your client version.";
                }
                Log("Could not connect to the server. " + advice, LogType::System);
                });

            // Executes as a response to LocationScouts.
            ap->set_location_info_handler([](const list<APClient::NetworkItem>& items) {
                for (const auto& item : items) {
                    if (ap->get_player_game(item.player) == ap->get_game() && !GameData::IsInteractable(item.location)) {
                        // interactable locations should have classification set by item classification only
                        GameData::SetPseudoItemClassification(item.location, item.item);
                    }
                    else if (item.flags & APClient::FLAG_ADVANCEMENT) {
                        GameData::SetOffWorldItemClassification(item.location, GameData::Classification::GenericProgression);
                    }
                    else if (item.flags & (APClient::FLAG_NEVER_EXCLUDE | APClient::FLAG_TRAP)) {
                        GameData::SetOffWorldItemClassification(item.location, GameData::Classification::GenericUsefulOrTrap);
                    }
                    else {
                        GameData::SetOffWorldItemClassification(item.location, GameData::Classification::GenericFiller);
                    }
                }
                });

            // Executes whenever items are received from the server.
            ap->set_items_received_handler([](const list<APClient::NetworkItem>& items) {
                for (const auto& item : items) {
                    Log(L"Receiving item with id " + std::to_wstring(item.item));
                    GameData::ReceiveItem(item.item);
                    Engine::SyncItems();
                }
                });

            // Executes whenever a chat message is received.
            ap->set_print_json_handler([](const APClient::PrintJSONArgs& args) {
                string plain_text = ap->render_json(args.data);
                string markdown_text = ProcessMessageText(args);
                Logger::PrintToConsole(
                    StringOps::ToWide(markdown_text),
                    StringOps::ToWide(plain_text)
                );

                if (args.type == "ItemSend") {
                    ReceiveItemOnce(args);
                    optional<Logger::ItemPopup> item_popup = BuildItemPopup(args);
                    if (item_popup) {
                        Logger::ShowPopup(*item_popup);
                    }
                    else {
                        Log(plain_text, LogType::Console);
                    }
                }
                else {
                    Log(plain_text, LogType::Console);
                }
                });

            // Executes whenever a bounce (such as a death link) is received.
            ap->set_bounced_handler([](const json& data) {
                Log("Receiving bounce: " + data.dump());

                auto tags = data.find("tags"); // This will either be data.end() or an array of tags.
                if (tags == data.end()) {
                    return; // Just ignore non-deathlink bounces.
                }

                bool is_deathlink = std::find(tags->begin(), tags->end(), "DeathLink") != tags->end();
                if (is_deathlink) {
                    ReceiveDeathLink(data);
                }
                });

            // Executes whenever the server tells us a location has been checked.
            ap->set_location_checked_handler([](const list<int64_t>& location_ids) {
                for (const auto& id : location_ids) {
                    Log(L"Marking location " + std::to_wstring(id) + L" as checked");
                    Despawn(id);
                }
                });
        } // End callbacks
    }

    void Client::Disconnect() {
        if (ap == nullptr) {
            return;
        }
        GameData::Close();
        delete ap;
        ap = nullptr;
        Log("Disconnected from Archipelago.", LogType::System);
    }

    void Client::SendCheck(int64_t id) {
        // TODO: Consider refactoring to queue location ids as an actual list
        list<int64_t> id_list{ id };
        Log(L"Sending check with id " + std::to_wstring(id));
        ap->LocationChecks(id_list);

        Log(L"Marking location " + std::to_wstring(id) + L" as checked");
        Despawn(id);
    }
    
    // Sets the data storage Zone value based on the player's current zone.
    void Client::SetZoneData() {
        if (ap == nullptr) {
            return;
        }

        string key =
            "Pseudoregalia - Team " + std::to_string(ap->get_team_number())
            + " - Player " + std::to_string(ap->get_player_number())
            + " - Zone";
        int32_t zone = static_cast<int32_t>(Engine::GetCurrentMap());
        list<APClient::DataStorageOperation> operations{ { "replace", zone } };
        ap->Set(key, 0, false, operations);
    }
    
    // Sends game completion flag to Archipelago.
    void Client::CompleteGame() {
        if (ap == nullptr) {
            // TODO: queue this if the player forgets to connect before winning
            return;
        }
        ap->StatusUpdate(APClient::ClientStatus::GOAL);

        // Send a key to datastorage upon game completion for PopTracker integration.
        string key =
            "Pseudoregalia - Team " + std::to_string(ap->get_team_number())
            + " - Player " + std::to_string(ap->get_player_number())
            + " - Game Complete";
        json default_value{ 0 };
        list<APClient::DataStorageOperation> filler_operations{ APClient::DataStorageOperation{ "add", default_value  } };
        ap->Set(key, default_value, true, filler_operations);
    }

    void Client::PollServer() {
        if (ap == nullptr) {
            return;
        }
        ap->poll();
    }

    void Client::SendDeathLink() {
        using DeathLinkMessages::RandomOutgoingDeathlink;
        using DeathLinkMessages::RandomOwnDeathlink;
        if (ap == nullptr
        || !Settings::GetDeathLink()
        || death_link_locked) {
            return;
        }

        string funny_message(std::vformat(RandomOutgoingDeathlink(), std::make_format_args(ap->get_slot())));
        json data{
            {"time", ap->get_server_time()},
            {"cause", funny_message},
            {"source", ap->get_slot()},
        };
        ap->Bounce(data, {}, {}, { "DeathLink" });
        Logger::ShowPopup(RandomOwnDeathlink());
        Log("Sending bounce: " + data.dump());
        Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
    }

    void Client::Say(string input) {
        if (ap == nullptr) {
            return;
        }

        ap->Say(input);
    }

    // returns true if the id param is a "missing location", ie a location that has an item and hasn't been checked. the
    // function returns false if not connected to indicate that there are no locations to check yet.
    bool Client::IsMissingLocation(int64_t id) {
        if (ap == nullptr) {
            return false;
        }
        return ap->get_missing_locations().contains(id);
    }

    vector<wstring> GetHintText(GameData::MajorKeyInfo info) {
        if (ap == nullptr) {
            return {};
        }

        if (info.locations.size() == 0) {
            return {};
        }

        using StringOps::ToWide;
        wstring key_name = ToWide(ap->get_item_name(info.item_id, ap->get_game()));
        if (info.found) {
            return { L"[#af99ef](" + key_name + L") has been found" };
        }

        vector<wstring> hints;
        for (const auto& loc : info.locations) {
            wstring location_name = ToWide(ap->get_location_name(loc.location_id, ap->get_player_game(loc.player_id)));
            wstring player_name = ToWide(ap->get_player_alias(loc.player_id));

            wstring hint = L"[#af99ef](" + key_name + L") is at [#00ff7f](" + location_name + L") in ";
            if (ap->slot_concerns_self(loc.player_id)) {
                hint += L"[#ee5fee](";
            }
            else {
                hint += L"[#fafa7f](";
            }
            hint += player_name + L")'s world";
            hints.push_back(hint);
        }
        return hints;
    }

    void CreateMajorKeyHints(GameData::MajorKeyInfo info) {
        if (ap == nullptr) {
            return;
        }

        if (info.locations.size() == 0) {
            Log("No hints found for key " + std::to_string(info.item_id));
            return;
        }

        unordered_map<int, list<int64_t>> player_to_key_locations;
        for (const auto& loc : info.locations) {
            player_to_key_locations[loc.player_id].push_back(loc.location_id);
        }
        for (const auto& [player_id, locations] : player_to_key_locations) {
            bool created_hints = ap->CreateHints(locations, player_id, APClient::HINT_PRIORITY);
            if (created_hints) {
                Log("Created hints for " + std::to_string(info.item_id) + " in " + ap->get_player_alias(player_id) +
                    "'s world");
                continue;
            }

            if (player_id == ap->get_player_number()) {
                ap->LocationScouts(locations, 2);
                Log("Scouted hints for " + std::to_string(info.item_id));
            }
            else {
                auto console_hint_messages = GetConsoleHintText(info.item_id, player_id, locations);
                for (const auto& [markdown, plain] : console_hint_messages) {
                    Logger::PrintToConsole(markdown, plain);
                }
                Log("Printed hints to console for " + std::to_string(info.item_id) + " in " +
                    ap->get_player_alias(player_id) + "'s world");
            }
        }
    }


    // Private functions
    namespace {
        string ProcessMessageText(const APClient::PrintJSONArgs& args) {
            string console_text;

            // This loop is basically the logic of APClient::render_json(), adapted to use RichTextBlock markdown.
            for (const auto& node : args.data) {
                size_t type_hash = StringOps::HashNstring(node.type);
                switch (type_hash) {
                case Hashes::player_id: {
                    int id = std::stoi(node.text);
                    string player_name = ap->get_player_alias(id);
                    if (id == ap->get_player_number()) {
                        console_text += "<Self>" + player_name + "</>";
                    }
                    else {
                        console_text += "<Player>" + player_name + "</>";
                    }
                    break;
                }
                case Hashes::item_id: {
                    int64_t id = std::stoll(node.text);
                    string item_name = ap->get_item_name(id, ap->get_player_game(node.player));
                    if (node.flags & APClient::FLAG_ADVANCEMENT) {
                        console_text += "<Progression";
                    }
                    else if (node.flags & APClient::FLAG_NEVER_EXCLUDE) {
                        console_text += "<Useful";
                    }
                    else if (node.flags & APClient::FLAG_TRAP) {
                        console_text += "<Trap";
                    }
                    else {
                        console_text += "<Filler";
                    }
                    console_text += "Item>" + item_name + "</>";
                    break;
                }
                case Hashes::location_id: {
                    int64_t id = std::stoll(node.text);
                    string location_name = ap->get_location_name(id, ap->get_player_game(node.player));
                    console_text += "<Location>" + location_name + "</>";
                    break;
                }
                default:
                    console_text += node.text;
                    break;
                }
            }

            return console_text;
        }

        optional<Logger::ItemPopup> BuildItemPopup(const APClient::PrintJSONArgs& args) {
            if (args.receiving == nullptr || args.item == nullptr) {
                // I don't think this is reachable because of the way apclientpp validates packets, but it felt weird
                // not to check just in case
                Log("Received ItemSend PrintJSON packet that was missing receiving or item.");
                return {};
            }

            int32_t receiver = *args.receiving;
            int32_t finder = args.item->player;
            int32_t player = ap->get_player_number();
            if (finder != player && receiver != player) {
                return {};
            }

            Logger::ItemPopup item_popup = {
                .item = StringOps::ToWide(ap->get_item_name(args.item->item, ap->get_player_game(receiver))),
            };
            wstring location = StringOps::ToWide(ap->get_location_name(args.item->location, ap->get_player_game(finder)));
            if (finder == player && receiver == player) {
                item_popup.preamble = L"Found ";
                item_popup.info = L"at " + location;
            }
            else if (finder == player) {
                item_popup.preamble = L"Sent ";
                wstring receiver_name = StringOps::ToWide(ap->get_player_alias(receiver));
                item_popup.info = L"to " + receiver_name + L" (" + location + L")";
            }
            else {
                item_popup.preamble = L"Received ";
                wstring finder_name = StringOps::ToWide(ap->get_player_alias(finder));
                item_popup.info = L"from " + finder_name + L" (" + location + L")";
            }
            return item_popup;
        }

        void ReceiveItemOnce(const APClient::PrintJSONArgs& args) {
            if (args.receiving == nullptr || args.item == nullptr) {
                return;
            }

            if (*args.receiving != ap->get_player_number()) {
                return;
            }

            GameData::ReceiveItemOnce(args.item->item);
        }

        void ReceiveDeathLink(const json& data) {
            if (ap == nullptr
                || !Settings::GetDeathLink()
                || death_link_locked) {
                return;
            }

            if (!data.contains("data")) {
                // Should only execute if the received death link data was not properly filled out.
                Logger::ShowPopup(L"You were assassinated by a mysterious villain...");
                Engine::VaporizeGoat();
                Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
                return;
            }
            json details = data["data"];
            string funny_message;

            if (details.contains("cause")) {
                string cause(details["cause"]);
                Logger::ShowPopup(cause);
            }
            else if (details.contains("source")) {
                string source(details["source"]);
                Logger::ShowPopup("You were brutally murdered by " + source + ".");
            }
            else {
                // Should only execute if the received death link data was not properly filled out.
                Logger::ShowPopup("You were assassinated by a mysterious villain...");
            }
            Engine::VaporizeGoat();
            Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
        }
        
        void Despawn(int64_t id) {
            if (GameData::IsInteractable(id)) {
                Engine::DespawnInteractable(id);
                return;
            }
            Engine::DespawnCollectible(id);
        }

        void ParseKeyHints(const json& hints) {
            for (int key_index = 0; key_index < 5; key_index++) {
                for (const auto& hint : hints[key_index]) {
                    GameData::AddMajorKeyHint(key_index, GameData::MultiworldLocation{
                        .player_id = hint["player"],
                        .location_id = hint["location"],
                    });
                }
            }
        }

        vector<pair<wstring, wstring>> GetConsoleHintText(int64_t item_id, int player_id, list<int64_t> location_ids) {
            using StringOps::ToWide;
            wstring key_name = ToWide(ap->get_item_name(item_id, ap->get_game()));
            vector<pair<wstring, wstring>> messages;
            for (const auto& location_id : location_ids) {
                wstring location_name = ToWide(ap->get_location_name(location_id, ap->get_player_game(player_id)));
                wstring player_name = ToWide(ap->get_player_alias(player_id));

                // we use <Player> and don't check to use <Self> because this function is only called for hints which
                // could not be created, which can only be hints for other player's locations
                wstring markdown = L"<ProgressionItem>" + key_name + L"</> is at <Location>" + location_name
                                   + L"</> in <Player>" + player_name + L"</>'s world";
                wstring plain = key_name + L" is at " + location_name + L" in " + player_name + L"'s world";

                messages.push_back({ markdown, plain });
            }
            return messages;
        }
    } // End private functions
}