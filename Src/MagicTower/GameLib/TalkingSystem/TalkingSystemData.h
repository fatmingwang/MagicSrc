#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <functional>

// Forward declaration
struct sDialogueNode;
struct sDialogueOption;
struct sDialogueBranch;
struct sDialogueContent;
struct sDialogueEntity;

//=============================================================================
// Dialogue Option - a choice the player can make
//=============================================================================
struct sDialogueOption
{
    std::string m_strLabelKey;              // localization key for option text
    int m_iNextNodeId = -1;                 // -1 = end conversation
    std::string m_strNextEntityId;          // empty = same entity, otherwise jump to different entity
    std::vector<std::string> m_vSetFlags;   // flags to set when chosen
    std::vector<std::string> m_vClearFlags; // flags to clear when chosen
    std::vector<std::string> m_vActions;    // actions to execute (e.g., "GiveItem:Gold:100")
};

//=============================================================================
// Dialogue Content - used by sequence/random/once/check types
//=============================================================================
struct sDialogueContent
{
    std::string m_strTextKey;               // localization key
    std::vector<sDialogueOption> m_vOptions;
    std::vector<std::string> m_vSetFlags;   // for "once" firstTime setFlags
};

//=============================================================================
// Dialogue Branch - used by conditional type
//=============================================================================
struct sDialogueBranch
{
    std::vector<std::string> m_vConditions; // required flags (empty = default)
    std::string m_strTextKey;
    std::vector<sDialogueOption> m_vOptions;
};

//=============================================================================
// Dialogue Node - one step in a conversation
//=============================================================================
enum class eDialogueNodeType
{
    Normal,
    Conditional,
    Sequence,
    Random,
    Once,
    Check
};

struct sDialogueNode
{
    int m_iNodeId = 0;
    eDialogueNodeType m_eType = eDialogueNodeType::Normal;
    std::string m_strSpeaker;
    
    // For "normal" type
    std::string m_strTextKey;
    std::vector<sDialogueOption> m_vOptions;
    
    // For "conditional" type
    std::vector<sDialogueBranch> m_vBranches;
    
    // For "sequence" type
    std::string m_strSequenceFlag;
    bool m_bLoop = true;
    std::vector<sDialogueContent> m_vContents;
    
    // For "once" type
    std::string m_strOnceFlag;
    sDialogueContent m_FirstTime;
    sDialogueContent m_Repeat;
    
    // For "check" type
    std::string m_strCheckType;  // e.g., "HasItem", "HasQuest"
    std::string m_strCheckParam; // e.g., "PassCard"
    sDialogueContent m_Pass;
    sDialogueContent m_Fail;
};

//=============================================================================
// Dialogue Entity - all nodes for one NPC/item
//=============================================================================
struct sDialogueEntity
{
    std::string m_strEntityId;
    std::string m_strAlias;
    std::unordered_map<int, sDialogueNode> m_mNodes; // nodeId -> node
};

//=============================================================================
// Localization Data
//=============================================================================
struct sLocalizationData
{
    std::string m_strVersion;
    // language code -> (key -> text)
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_mLanguages;
};

//=============================================================================
// Dialogue Data - all entities loaded from JSON
//=============================================================================
struct sDialogueData
{
    std::string m_strVersion;
    std::unordered_map<std::string, sDialogueEntity> m_mEntities; // entityId -> entity
};

//=============================================================================
// Current Dialogue State - runtime state for active conversation
//=============================================================================
struct sDialogueState
{
    std::string m_strCurrentEntityId;
    int m_iCurrentNodeId = -1;
    bool m_bIsActive = false;
    
    // Resolved content for current node (after evaluating conditions/sequence/etc.)
    std::string m_strResolvedTextKey;
    std::string m_strResolvedSpeaker;
    std::vector<sDialogueOption> m_vResolvedOptions;
};

//=============================================================================
// Action Callback - function signature for action handlers
//=============================================================================
using FlagCheckCallback = std::function<bool(const std::string&)>;
using ActionCallback = std::function<void(const std::string&)>;
using ItemCheckCallback = std::function<bool(const std::string&, const std::string&)>;
