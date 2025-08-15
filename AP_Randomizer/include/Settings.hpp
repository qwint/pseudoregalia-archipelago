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

    namespace Filters {
        enum class ItemSend {
            All,
            Relevant,
            None
        };
    }

    void Load();
    ItemDisplay GetItemDisplay();
    InteractableAuraDisplay GetInteractableAuraDisplay();
    bool GetDeathLink();
    PopupsInitialState GetPopupsInitialState();
    Filters::ItemSend GetItemSendFilter();
}
