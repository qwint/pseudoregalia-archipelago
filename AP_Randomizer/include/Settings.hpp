#pragma once

namespace Settings {
    enum class ItemDisplay {
        Full,
        GenericNonPseudo,
        GenericAll,
    };

    enum class PopupsInitialState {
        ShowWithSound,
        ShowMuted,
        Hide,
    };

    void Load();
    ItemDisplay GetItemDisplay();
    bool GetDeathLink();
    PopupsInitialState GetPopupsInitialState();
    bool GetPopupsSimplifyItemFont();
}
