#define NOMINMAX
#pragma once

#include <Windows.h>
#include "Mod/CppUserModBase.hpp"
#include "Unreal/UObjectGlobals.hpp"
#include "Unreal/Hooks.hpp"
#include "Unreal/UFunction.hpp"
#include "Unreal/AActor.hpp"
#include "Unreal/FText.hpp"
#include "Unreal/UClass.hpp"
#include "NameTypes.hpp"
#include "Client.hpp"
#include "UnrealConsole.hpp"
#include "Engine.hpp"
#include "Logger.hpp"
#include "Timer.hpp"
#include "StringOps.hpp"
#include "Settings.hpp"

class AP_Randomizer : public RC::CppUserModBase {
public:
    struct BoundKey {
        int key;
        std::function<void()> callback;
        bool isPressed = false;
    };
    bool returncheck_hooked = false;
    bool toggleslidejump_hooked = false;
    bool deathlink_hooked = false;
    bool manageresult_hooked = false;
    bool copytext_hooked = false;
    bool sendmessage_hooked = false;
    bool npc_endinteract_hooked = false;
    bool chair_tryinteract_hooked = false;
    bool book_endinteract_hooked = false;
    bool note_tryinteract_hooked = false;
    bool finishnote_hooked = false;

    AP_Randomizer() : CppUserModBase() {
        ModName = STR("AP_Randomizer");
        ModVersion = STR("0.1.0");
        ModDescription = STR("archipelago randomizer for pseudoregalia");
        ModAuthors = STR("littlemeowmeow0134");
        //ModIntendedSDKVersion = STR("2.6");

        Settings::Load();
    }

    ~AP_Randomizer()
    {
    }

    auto on_unreal_init() -> void override {
        using namespace RC::Unreal;

        // We need to force blueprint mods to load as soon as possible.
        // Sending an Insert keypress once UE initializes will tell UE4SS' BPModLoaderMod to load them manually.
        {
            INPUT inputs[1] = {};
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.wVk = VK_INSERT;
            SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
        }

        // I want to make this an AActorTickCallback hook so I can only check actor name,
        // but for some reason that doesn't seem to respond.
        Hook::RegisterProcessEventPreCallback([&](UObject* object, UFunction* function, void* params) {
            static FName randomizer_instance = FName(STR("BP_APRandomizerInstance_C"), RC::Unreal::FNAME_Add);
            static FName receive_tick = FName(STR("ReceiveTick"), RC::Unreal::FNAME_Add);

            bool is_main_randomizer_blueprint = object->GetClassPrivate()->GetNamePrivate() == randomizer_instance;
            bool is_event_tick = function->GetNamePrivate() == receive_tick;

            if (is_main_randomizer_blueprint && is_event_tick) {
                float* delta_seconds = static_cast<float*>(params);
                Timer::OnTick(*delta_seconds);
                Engine::OnTick(object);
            }
            });

        Hook::RegisterProcessConsoleExecCallback([&](UObject* object, const Unreal::TCHAR* command, FOutputDevice& Ar, UObject* executor) -> bool {
            if (command[0] == '/' || command[0] == '!') {
                command++; // Exclude the first character from the array
                UnrealConsole::ProcessCommand(command);
                return true;
            }
            return PropogateCommand(command);
            });

        Hook::RegisterBeginPlayPostCallback([&](AActor* actor) {
            // TODO: Consider moving some of this function out of main
            auto returncheck = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                Client::SendCheck(context.GetParams<int64_t>());
                };
            auto toggleslidejump = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                Engine::ToggleSlideJump();
                };
            auto deathlink = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                Client::SendDeathLink();
                };
            auto spawntimetrialcollectible = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                Engine::SpawnTimeTrialCollectibleIfBeaten(context.Context);
                };
            auto interact = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                GameData::Interact(context.Context->GetName());
            };
            auto readnote = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                GameData::ReadNote(context.Context->GetName());
            };

            if (!returncheck_hooked
                && actor->GetName().starts_with(STR("BP_APCollectible"))) {

                UFunction* return_check_function = actor->GetFunctionByName(STR("ReturnCheck"));
                if (!return_check_function) {
                    Log(L"Could not find function \"ReturnCheck\" in BP_APCollectible.", LogType::Error);
                    return;
                }
                else {
                    Log(L"Establishing hook for ReturnCheck.");
                }
                Unreal::UObjectGlobals::RegisterHook(return_check_function, EmptyFunction, returncheck, nullptr);
                returncheck_hooked = true;
            }

            if (actor->GetName().starts_with(STR("BP_APRandomizerInstance"))) {
                if (!toggleslidejump_hooked) {
                    UFunction* toggle_function = actor->GetFunctionByName(L"AP_ToggleSlideJump");
                    if (!toggle_function) {
                        Log(L"Could not find function \"AP_ToggleSlideJump\" in BP_APRandomizerInstance.", LogType::Error);
                        return;
                    }
                    else {
                        Log(L"Establishing hook for AP_ToggleSlideJump.");
                    }
                    Unreal::UObjectGlobals::RegisterHook(toggle_function, EmptyFunction, toggleslidejump, nullptr);
                    toggleslidejump_hooked = true;
                }
                // TODO: see if i can make this work in a way that doesn't suck
                // Log(L"Loaded scene " + Engine::GetCurrentMap());
                if (Engine::GetCurrentMap() == GameData::Map::EndScreen) {
                    Client::CompleteGame();
                }
                if (Engine::GetCurrentMap() == GameData::Map::TitleScreen) {
                    Client::Disconnect();
                }
                Engine::SpawnCollectibles();
                Engine::SyncItems();
                Client::SetZoneData();
            }

            if (actor->GetName().starts_with(L"BP_PlayerGoatMain")) {
                if (!deathlink_hooked) {
                    UFunction* death_function = actor->GetFunctionByName(L"BPI_CombatDeath");
                    if (!death_function) {
                        Log(L"Could not find function \"BPI_CombatDeath\" in BP_PlayerGoatMain.", LogType::Error);
                        return;
                    }
                    else {
                        Log(L"Establishing hook for BPI_CombatDeath.");
                    }
                    Unreal::UObjectGlobals::RegisterHook(death_function, EmptyFunction, deathlink, nullptr);
                    deathlink_hooked = true;
                }
            }

            if (actor->GetName().starts_with(L"BP_TimeTrial")) {
                if (!manageresult_hooked) {
                    UFunction* manageresult_function = actor->GetFunctionByName(L"manageResult");
                    if (!manageresult_function) {
                        Log(L"Could not find function \"manageResult\" in BP_TimeTrial.", LogType::Error);
                        return;
                    }
                    Log(L"Establishing hook for manageResult");
                    Unreal::UObjectGlobals::RegisterHook(manageresult_function, EmptyFunction, spawntimetrialcollectible, nullptr);
                    manageresult_hooked = true;
                }
            }

            if (actor->GetFullName().starts_with(L"BP_NPC_C ")) {
                if (!npc_endinteract_hooked) {
                    UFunction* endinteract_function = actor->GetFunctionByName(L"BPI_EndInteract");
                    if (!endinteract_function) {
                        Log(L"Could not find function \"BPI_EndInteract\" in BP_NPC.", LogType::Error);
                        return;
                    }
                    Log("Establishing hook for BPI_EndInteract");
                    Unreal::UObjectGlobals::RegisterHook(endinteract_function, EmptyFunction, interact, nullptr);
                    npc_endinteract_hooked = true;
                }
            }

            if (actor->GetFullName().starts_with(L"BP_RestChair_C ")) {
                if (!chair_tryinteract_hooked) {
                    UFunction* tryinteract_function = actor->GetFunctionByName(L"BPI_TryInteract");
                    if (!tryinteract_function) {
                        Log(L"Could not find function \"BPI_TryInteract\" in BP_RestChair.", LogType::Error);
                        return;
                    }
                    Log("Establishing hook for BPI_TryInteract");
                    Unreal::UObjectGlobals::RegisterHook(tryinteract_function, EmptyFunction, interact, nullptr);
                    chair_tryinteract_hooked = true;
                }
            }

            if (actor->GetFullName().starts_with(L"BP_ExamineTextPopup_C ")) {
                if (!book_endinteract_hooked) {
                    UFunction* endinteract_function = actor->GetFunctionByName(L"BPI_EndInteract");
                    if (!endinteract_function) {
                        Log(L"Could not find function \"BPI_EndInteract\" in BP_ExamineTextPopup_C.", LogType::Error);
                        return;
                    }
                    Log("Establishing hook for BPI_EndInteract");
                    Unreal::UObjectGlobals::RegisterHook(endinteract_function, EmptyFunction, interact, nullptr);
                    book_endinteract_hooked = true;
                }
            }

            if (actor->GetFullName().starts_with(L"BP_Note_C ")) {
                if (!note_tryinteract_hooked) {
                    UFunction* tryinteract_function = actor->GetFunctionByName(L"BPI_TryInteract");
                    if (!tryinteract_function) {
                        Log(L"Could not find function \"BPI_TryInteract\" in BP_Note_C.", LogType::Error);
                        return;
                    }
                    Log("Establishing hook for BPI_TryInteract");
                    Unreal::UObjectGlobals::RegisterHook(tryinteract_function, EmptyFunction, readnote, nullptr);
                    note_tryinteract_hooked = true;
                }

                FText* inText = static_cast<FText*>(actor->GetValuePtrByPropertyName(L"inText"));
                inText->SetString(FString(L"I got some new shoes today, the seller said they're supposed to be great for parkour?\r\n\r\n\"These kicks will have you moving around like a laserbeam in a hall of mirrors!\r\n\r\nForget what you know,\r\ncarefully consider the angle of your jump before you make it.\r\nOnly by reflecting will you become a master of movement!\"\r\n\r\nHe really didn't need to give me all that though, i just thought they looked cool..."));
            }
            });

        Hook::RegisterStaticConstructObjectPostCallback([&](const FStaticConstructObjectParameters& params, UObject* object) -> UObject* {
            // Copies text in highlighted message to clipboard.
            auto copytext = [&](UnrealScriptFunctionCallableContext& context, void* customdata) {
                std::wstring wide(context.GetParams<FText>().ToString());

                // Shamelessly copied from https://stackoverflow.com/questions/40664890/copy-unicode-string-to-clipboard-isnt-working
                // I have no idea how this works lol.
                const wchar_t* buffer = wide.c_str();
                size_t size = sizeof(WCHAR) * (wcslen(buffer) + 1);
                if (!OpenClipboard(0)) {
                    Log("Could not open clipboard!", LogType::Warning);
                    return;
                }
                HGLOBAL hClipboardData = GlobalAlloc(GMEM_MOVEABLE, size);
                WCHAR* pchData;
                pchData = (WCHAR*)GlobalLock(hClipboardData);
                wcscpy_s(pchData, size / sizeof(wchar_t), buffer);
                GlobalUnlock(hClipboardData);
                SetClipboardData(CF_UNICODETEXT, hClipboardData);
                CloseClipboard();
                };

            auto sendmessage = [&](UnrealScriptFunctionCallableContext& context, void* customdata) {
                FText input = context.GetParams<FText>();
                UnrealConsole::ProcessInput(input);
                };
            auto finishnote = [](UnrealScriptFunctionCallableContext& context, void* customdata) {
                GameData::FinishNote();
            };

            // I'm not sure why, but this triggers on an object with the name "AP_DeluxeConsole_C" and one with a name
            // like "AP_DeluxeConsole_C_{bunch of numbers}". it seems like the one with the numbers is the "real" one,
            // as the other one doesn't have all the functions that the console actually has. it might have to do with
            // how widgets work, but I don't know the details.
            if (object->GetName().starts_with(L"AP_DeluxeConsole_C_")) {
                if (!copytext_hooked) {
                    UFunction* copy_function = object->GetFunctionByName(STR("AP_CopyToClipboard"));
                    if (!copy_function) {
                        // For some reason this always fails once so don't bother displaying an error.
                        Logger::Log(L"Could not find function \"AP_CopyToClipboard\" in AP_DeluxeConsole.");
                        return object;
                    }
                    else {
                        Logger::Log(L"Registering hook for AP_CopyToClipboard.");
                    }
                    Unreal::UObjectGlobals::RegisterHook(copy_function, copytext, EmptyFunction, nullptr);
                    copytext_hooked = true;
                }

                if (!sendmessage_hooked) {
                    UFunction* send_function = object->GetFunctionByName(STR("AP_SendMessage"));
                    if (!send_function) {
                        Log(L"Could not find function \"AP_SendMessage\" in AP_DeluxeConsole.");
                        return object;
                    }
                    else {
                        Log(L"Registering hook for AP_SendMessage.");
                    }
                    Unreal::UObjectGlobals::RegisterHook(send_function, sendmessage, EmptyFunction, nullptr);
                    sendmessage_hooked = true;
                }

                Engine::InitializeConsole(object);
            }

            if (object->GetName().starts_with(L"UI_Note_C_")) {
                if (!finishnote_hooked) {
                    UFunction* click_button_function = object->GetFunctionByName(L"BndEvt__UI_Note_UI_GenericButton_K2Node_ComponentBoundEvent_0_CommonButtonBaseClicked__DelegateSignature");
                    UFunction* text_advance_function = object->GetFunctionByName(L"BPI_TextAdvance");
                    UFunction* close_self_function = object->GetFunctionByName(L"Close Self");
                    if (!click_button_function || !text_advance_function || !close_self_function) {
                        Log(L"Could not find functions in UI_Note", LogType::Error);
                        return object;
                    }

                    Log("Registering hooks for UI_Note");
                    Unreal::UObjectGlobals::RegisterHook(click_button_function, EmptyFunction, finishnote, nullptr);
                    Unreal::UObjectGlobals::RegisterHook(text_advance_function, EmptyFunction, finishnote, nullptr);
                    Unreal::UObjectGlobals::RegisterHook(close_self_function, EmptyFunction, finishnote, nullptr);
                    finishnote_hooked = true;
                }
            }

            return object;
            });

        setup_keybinds();
    }

    bool PropogateCommand(const Unreal::TCHAR* command) {
        // Some console commands do dramatic things like crash on command or simulating debug scenarios.
        // Only "reconnect" and "disconnect" are forbidden from propogating for now since they're easy to mistakenly enter.
        // I haven't decided yet whether more obscure commands should be eaten here as well.
        if (*command == *STR("disconnect") || *command == *STR("reconnect")) {
            return true;
        }
        return false;
    }

    auto on_update() -> void override
    {
        Client::PollServer();
        Logger::OnTick();
        for (auto& boundKey : m_boundKeys)
        {
            if ((GetKeyState(boundKey.key) & 0x8000) && !boundKey.isPressed)
            {
                boundKey.isPressed = true;
            }
            if (!(GetKeyState(boundKey.key) & 0x8000) && boundKey.isPressed)
            {
                boundKey.isPressed = false;
                boundKey.callback();
            }
        }
    }

    auto bind_key(const int& keyCode, const std::function<void()>& callback) -> void {
        BoundKey newBoundKey{
            .key = keyCode,
            .callback = callback,
            .isPressed = false
        };
        m_boundKeys.push_back(newBoundKey);
    }

    auto setup_keybinds() -> void
    {
        // List of key codes at https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
        bind_key(VK_NUMPAD1, [&]() {
            // Client::CompleteGame();
            // Client::Connect("localhost:38281", "goat", "");
            });

        bind_key(VK_NUMPAD2, [&]() {
            });

        bind_key(VK_NUMPAD7, [&]() {
            });

        bind_key(VK_NUMPAD8, [&]() {
            });
    }

    static void EmptyFunction(RC::Unreal::UnrealScriptFunctionCallableContext& context, void* customdata) {
        // Empty function to provide to RegisterHook.
    }

private:
    std::vector<BoundKey> m_boundKeys;
    std::unordered_set<int> m_pressedKeys;
};

#define AP_RANDOMIZER_API __declspec(dllexport)
extern "C"
{
    AP_RANDOMIZER_API RC::CppUserModBase* start_mod()
    {
        return new AP_Randomizer();
    }

    AP_RANDOMIZER_API void uninstall_mod(RC::CppUserModBase* mod)
    {
        delete mod;
    }
}