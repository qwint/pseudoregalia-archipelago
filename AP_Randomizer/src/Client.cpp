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
#include "Unreal/FText.hpp"
#include "GameData.hpp"
#include "Engine.hpp"
#include "Client.hpp"
#include "Logger.hpp"
#include "Timer.hpp"
#include "DeathLinkMessages.hpp"
#include "StringOps.hpp"
#include "Settings.hpp"

namespace Client {
    using std::string;
    using std::list;

    namespace Hashes {
        using StringOps::HashNstring;
        constexpr size_t player_id = HashNstring("player_id");
        constexpr size_t item_id = HashNstring("item_id");
        constexpr size_t location_id = HashNstring("location_id");
    }

    // Private members
    namespace {
        struct MessageInfo {
            string markdown_text;
            bool mentions_player;
        };

        typedef nlohmann::json json;
        typedef APClient::State ConnectionStatus;
        void ReceiveItems(const list<APClient::NetworkItem>&);
        MessageInfo ProcessMessageText(const APClient::PrintJSONArgs&);
        void ReceiveDeathLink(const json&);

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
                APClient::Version version{ 0, 7, 0 };
                ap->ConnectSlot(slot_name, password, items_handling, {}, version);
                });

            // Executes on successful connection to slot.
            ap->set_slot_connected_handler([](const json& slot_data) {
                Log("Connected to slot");
                if (Settings::GetDeathLink()) {
                    ap->ConnectUpdate(false, 0, true, list<string> {"DeathLink"});
                }
                for (json::const_iterator iter = slot_data.begin(); iter != slot_data.end(); iter++) {
                    GameData::SetOption(iter.key(), iter.value());
                }
                ap->LocationScouts(GameData::GetLocations());
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
                    if (ap->get_player_game(item.player) == ap->get_game()) {
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
                using RC::Unreal::FText;
                string plain_text = ap->render_json(args.data);
                MessageInfo info = ProcessMessageText(args);
                Logger::PrintToConsole(
                    StringOps::ToWide(info.markdown_text),
                    StringOps::ToWide(plain_text)
                );

                if (args.type == "ItemSend") {
                    Log(plain_text, LogType::Popup, info.mentions_player);
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
                    Engine::DespawnCollectible(id);
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
    }

    void Client::SendCheck(int64_t id) {
        // TODO: Consider refactoring to queue location ids as an actual list
        list<int64_t> id_list{ id };
        Log(L"Sending check with id " + std::to_wstring(id));
        ap->LocationChecks(id_list);

        Log(L"Marking location " + std::to_wstring(id) + L" as checked");
        Engine::DespawnCollectible(id);
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
        Log(RandomOwnDeathlink(), LogType::Popup);
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


    // Private functions
    namespace {
        MessageInfo ProcessMessageText(const APClient::PrintJSONArgs& args) {
            string console_text;
            bool mentions_player = false;

            // This loop is basically the logic of APClient::render_json(), adapted to use RichTextBlock markdown.
            for (const auto& node : args.data) {
                size_t type_hash = StringOps::HashNstring(node.type);
                switch (type_hash) {
                case Hashes::player_id: {
                    int id = std::stoi(node.text);
                    string player_name = ap->get_player_alias(id);
                    if (id == ap->get_player_number()) {
                        mentions_player = true;
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

            return MessageInfo{ console_text, mentions_player };
        }

        void ReceiveDeathLink(const json& data) {
            if (ap == nullptr
                || !Settings::GetDeathLink()
                || death_link_locked) {
                return;
            }

            if (!data.contains("data")) {
                // Should only execute if the received death link data was not properly filled out.
                Log("You were assassinated by a mysterious villain...", LogType::Popup);
                Engine::VaporizeGoat();
                Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
                return;
            }
            json details = data["data"];
            string funny_message;

            if (details.contains("cause")) {
                string cause(details["cause"]);
                Log(cause, LogType::Popup);
            }
            else if (details.contains("source")) {
                string source(details["source"]);
                Log("You were brutally murdered by " + source + ".", LogType::Popup);
            }
            else {
                // Should only execute if the received death link data was not properly filled out.
                Log("You were assassinated by a mysterious villain...", LogType::Popup);
            }
            Engine::VaporizeGoat();
            Timer::RunTimerInGame(death_link_timer_seconds, &death_link_locked);
        }
    } // End private functions
}