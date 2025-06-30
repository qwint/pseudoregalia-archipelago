#pragma once

namespace Settings {
    enum class ItemDisplay {
        Full,
        GenericNonPseudo,
        GenericAll,
    };

    enum class Popups {
        ShowWithSound,
        ShowMuted,
        Hide,
    };

    void Load();
    ItemDisplay GetItemDisplay();
    Popups GetPopups();
    bool GetDeathLink();
}
