#pragma once

#include "TalkingSystemData.h"
#include "DialogueParser.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <random>

//=============================================================================
// cTalkSystemBehaviorController
// Manages dialogue flow: loads data, tracks state, resolves nodes, handles choices
//=============================================================================
class cTalkSystemBehaviorController
{
public:
    cTalkSystemBehaviorController();
    ~cTalkSystemBehaviorController();

    //=========================================================================
    // Initialization
    //=========================================================================
    
    // Load dialogue structure from JSON file
    bool LoadDialogueFile(const std::string& e_strFilePath);
    
    // Load localization from JSON file
    bool LoadLocalizationFile(const std::string& e_strFilePath);
    
    // Set current language (e.g., "en", "zh_TW", "ja")
    void SetLanguage(const std::string& e_strLangCode);
    const std::string& GetLanguage() const { return m_strCurrentLanguage; }

    //=========================================================================
    // Conversation Control
    //=========================================================================
    
    // Start conversation with an entity by ID
    // Returns true if conversation started successfully
    bool StartConversation(const std::string& e_strEntityId);
    
    // Choose an option by index (0-based)
    // Returns true if conversation continues, false if ended
    bool ChooseOption(int e_iOptionIndex);
    
    // End current conversation manually
    void EndConversation();
    
    // Check if a conversation is currently active
    bool IsConversationActive() const { return m_State.m_bIsActive; }

    //=========================================================================
    // Current State Access
    //=========================================================================
    
    // Get current dialogue state (for UI rendering)
    const sDialogueState& GetCurrentState() const { return m_State; }
    
    // Get resolved text for current node (localized)
    std::string GetCurrentText() const;
    
    // Get resolved speaker name
    const std::string& GetCurrentSpeaker() const { return m_State.m_strResolvedSpeaker; }
    
    // Get resolved options (for UI)
    const std::vector<sDialogueOption>& GetCurrentOptions() const { return m_State.m_vResolvedOptions; }
    
    // Get localized text for an option
    std::string GetOptionText(int e_iOptionIndex) const;
    
    // Get number of available options
    int GetOptionCount() const { return static_cast<int>(m_State.m_vResolvedOptions.size()); }

    //=========================================================================
    // Flag Management (Persistent State)
    //=========================================================================
    
    // Check if a flag is set
    bool HasFlag(const std::string& e_strFlag) const;
    
    // Set a flag
    void SetFlag(const std::string& e_strFlag);
    
    // Clear a flag
    void ClearFlag(const std::string& e_strFlag);
    
    // Get all flags (for save/load)
    const std::unordered_set<std::string>& GetAllFlags() const { return m_setFlags; }
    
    // Set all flags (for save/load)
    void SetAllFlags(const std::unordered_set<std::string>& e_setFlags) { m_setFlags = e_setFlags; }

    //=========================================================================
    // Sequence Index Management (for sequence node type)
    //=========================================================================
    
    int GetSequenceIndex(const std::string& e_strSequenceFlag) const;
    void SetSequenceIndex(const std::string& e_strSequenceFlag, int e_iIndex);
    
    // Get all sequence indices (for save/load)
    const std::unordered_map<std::string, int>& GetAllSequenceIndices() const { return m_mSequenceIndices; }
    void SetAllSequenceIndices(const std::unordered_map<std::string, int>& e_mIndices) { m_mSequenceIndices = e_mIndices; }

    //=========================================================================
    // Callbacks - Register handlers for game integration
    //=========================================================================
    
    // Register action handler (called when option with action is chosen)
    // Action format: "ActionName:Param1:Param2" e.g., "GiveItem:Gold:100"
    void SetActionCallback(ActionCallback e_Callback) { m_ActionCallback = e_Callback; }
    
    // Register item check handler (for "check" node type with HasItem)
    void SetItemCheckCallback(ItemCheckCallback e_Callback) { m_ItemCheckCallback = e_Callback; }
    
    // Register custom flag check (allows external flag sources)
    void SetExternalFlagCheckCallback(FlagCheckCallback e_Callback) { m_ExternalFlagCheckCallback = e_Callback; }

    //=========================================================================
    // Debug / Utility
    //=========================================================================
    
    // Get entity alias by ID
    std::string GetEntityAlias(const std::string& e_strEntityId) const;
    
    // Get last error message
    const std::string& GetLastError() const { return m_strLastError; }
    
    // Check if entity exists
    bool HasEntity(const std::string& e_strEntityId) const;

private:
    //=========================================================================
    // Internal Methods
    //=========================================================================
    
    // Resolve current node based on type (evaluates conditions, sequence, etc.)
    bool ResolveCurrentNode();
    
    // Check if all conditions in a list are satisfied
    bool CheckConditions(const std::vector<std::string>& e_vConditions) const;
    
    // Check a single condition (supports "!" prefix for negation)
    bool CheckSingleCondition(const std::string& e_strCondition) const;
    
    // Perform check for "check" node type
    bool PerformCheck(const std::string& e_strCheckType, const std::string& e_strCheckParam) const;
    
    // Apply option effects (set/clear flags, execute actions)
    void ApplyOptionEffects(const sDialogueOption& e_Option);
    
    // Navigate to a specific entity and node
    bool NavigateTo(const std::string& e_strEntityId, int e_iNodeId);
    
    // Get localized string by key
    std::string GetLocalizedString(const std::string& e_strKey) const;

    //=========================================================================
    // Member Variables
    //=========================================================================
    
    // Loaded data
    sDialogueData m_DialogueData;
    sLocalizationData m_LocalizationData;
    
    // Current state
    sDialogueState m_State;
    std::string m_strCurrentLanguage;
    
    // Persistent flags
    std::unordered_set<std::string> m_setFlags;
    
    // Sequence indices (sequenceFlag -> current index)
    std::unordered_map<std::string, int> m_mSequenceIndices;
    
    // Callbacks
    ActionCallback m_ActionCallback;
    ItemCheckCallback m_ItemCheckCallback;
    FlagCheckCallback m_ExternalFlagCheckCallback;
    
    // Parser
    cDialogueParser m_Parser;
    
    // Error handling
    std::string m_strLastError;
    
    // Random generator for "random" node type
    mutable std::mt19937 m_RandomEngine;
};
