#pragma once

namespace Settings {
    enum class ItemDisplay {
        Full,
        GenericNonPseudo,
        GenericAll,
    };

    void Load();
    ItemDisplay GetItemDisplay();
    bool GetDeathLink();
}
