# Talking System — Design & Examples

This folder contains the talking/dialogue system plan and example data for a data-driven RPG-style conversation system. The data is keyed by entity ID (NPC or item) and supports conditional branches, flags, and actions.

## Goals
- Trigger dialogue UI when player clicks an NPC or item (by entity ID).
- Keep dialogue data in JSON files, authored by designers/writers.
- Support conditions (flags, inventory, quest state) and actions (set flags, give item, start quest).
- Separate data -> manager -> UI -> action dispatcher.

## Key components
- Loader & validator: parse JSON and validate links (missing nodes, invalid ids).
- Dialogue manager: holds current conversation state and flag sets (persistent/temporary).
- Condition evaluator: checks required flags or simple boolean expressions.
- Action dispatcher: executes data-driven actions (GiveItem, StartQuest, OpenShop).
- UI: modal overlay showing speaker portrait, text, and selectable options.

## Implementation (Code)

Source files located at: `Src/MagicTower/GameLib/TalkingSystem/`

| File | Description |
|------|-------------|
| `TalkingSystemData.h` | Data structures (sDialogueNode, sDialogueOption, etc.) |
| `DialogueParser.h/cpp` | JSON parser using nlohmann/json |
| `TalkSystemBehaviorController.h/cpp` | Main controller for conversation flow |

### Usage Example

```cpp
#include "TalkingSystem/TalkSystemBehaviorController.h"

// Create controller
cTalkSystemBehaviorController l_TalkController;

// Load data
l_TalkController.LoadDialogueFile("Data/dialogue_example.json");
l_TalkController.LoadLocalizationFile("Data/dialogue_localization.json");
l_TalkController.SetLanguage("en");

// Register action handler
l_TalkController.SetActionCallback([](const std::string& e_strAction) {
    // Parse action: "GiveItem:Gold:100" -> give 100 gold
    // Implement your game logic here
});

// Register item check for "check" node type
l_TalkController.SetItemCheckCallback([](const std::string& e_strType, const std::string& e_strParam) {
    if (e_strType == "HasItem") {
        // Check if player has item e_strParam
        return g_Inventory.HasItem(e_strParam);
    }
    return false;
});

// Start conversation when player clicks NPC
void OnNPCClicked(const std::string& e_strEntityId) {
    if (l_TalkController.StartConversation(e_strEntityId)) {
        ShowDialogueUI();
    }
}

// In dialogue UI update
void UpdateDialogueUI() {
    if (!l_TalkController.IsConversationActive()) {
        HideDialogueUI();
        return;
    }
    
    // Display speaker and text
    std::string l_strSpeaker = l_TalkController.GetCurrentSpeaker();
    std::string l_strText = l_TalkController.GetCurrentText();
    
    // Display options
    int l_iOptionCount = l_TalkController.GetOptionCount();
    for (int i = 0; i < l_iOptionCount; ++i) {
        std::string l_strOptionText = l_TalkController.GetOptionText(i);
        // Render option button
    }
}

// When player clicks an option
void OnOptionClicked(int e_iOptionIndex) {
    bool l_bContinues = l_TalkController.ChooseOption(e_iOptionIndex);
    if (!l_bContinues) {
        HideDialogueUI();
    }
}

// Save/Load flags
void SaveGame() {
    auto l_setFlags = l_TalkController.GetAllFlags();
    auto l_mSeqIndices = l_TalkController.GetAllSequenceIndices();
    // Serialize to save file
}

void LoadGame() {
    // Deserialize from save file
    l_TalkController.SetAllFlags(l_setFlags);
    l_TalkController.SetAllSequenceIndices(l_mSeqIndices);
}
```

## Files in this folder
- `TalkingSystemPlan.md` — this document.
- `examples/dialogue_example.json` — small example JSON data set.
- `examples/dialogue_example.md` — explanation of JSON fields and usage.
- `examples/dialogue_localization.json` — multi-language string table.

## Notes
- Start small: single-flag conditions and single-flag option side-effects.
- Use localization keys (`textKey`, `labelKey`) for multi-language support.
- Persist flags and sequence indices with existing save system when ready.


