#pragma once

namespace Settings {
    enum class ItemDisplay {
        Full,
        GenericNonPseudo,
        GenericAll,
    };

    enum class InteractableAuraDisplay {
        Classification,
        Generic,
        None,
    };

    enum class PopupsInitialState {
        ShowWithSound,
        ShowMuted,
        Hide,
    };

    void Load();
    ItemDisplay GetItemDisplay();
    InteractableAuraDisplay GetInteractableAuraDisplay();
    bool GetDeathLink();
    PopupsInitialState GetPopupsInitialState();
    bool GetPopupsSimplifyItemFont();
}
