#include "TalkSystemBehaviorController.h"
#include <chrono>

//=============================================================================
// Constructor / Destructor
//=============================================================================
cTalkSystemBehaviorController::cTalkSystemBehaviorController()
    : m_strCurrentLanguage("en")
{
    // Seed random engine with current time
    auto l_Seed = static_cast<unsigned int>(
        std::chrono::steady_clock::now().time_since_epoch().count()
    );
    m_RandomEngine.seed(l_Seed);
}

cTalkSystemBehaviorController::~cTalkSystemBehaviorController()
{
}

//=============================================================================
// LoadDialogueFile
//=============================================================================
bool cTalkSystemBehaviorController::LoadDialogueFile(const std::string& e_strFilePath)
{
    if (!m_Parser.ParseDialogueFile(e_strFilePath, m_DialogueData))
    {
        m_strLastError = m_Parser.GetLastError();
        return false;
    }
    return true;
}

//=============================================================================
// LoadLocalizationFile
//=============================================================================
bool cTalkSystemBehaviorController::LoadLocalizationFile(const std::string& e_strFilePath)
{
    if (!m_Parser.ParseLocalizationFile(e_strFilePath, m_LocalizationData))
    {
        m_strLastError = m_Parser.GetLastError();
        return false;
    }
    return true;
}

//=============================================================================
// SetLanguage
//=============================================================================
void cTalkSystemBehaviorController::SetLanguage(const std::string& e_strLangCode)
{
    m_strCurrentLanguage = e_strLangCode;
}

//=============================================================================
// StartConversation
//=============================================================================
bool cTalkSystemBehaviorController::StartConversation(const std::string& e_strEntityId)
{
    // Check if entity exists
    auto l_It = m_DialogueData.m_mEntities.find(e_strEntityId);
    if (l_It == m_DialogueData.m_mEntities.end())
    {
        m_strLastError = "Entity not found: " + e_strEntityId;
        return false;
    }
    
    const sDialogueEntity& l_Entity = l_It->second;
    
    // Find starting node (usually nodeId 0, or first available)
    int l_iStartNodeId = -1;
    
    // Try node 0 first
    if (l_Entity.m_mNodes.find(0) != l_Entity.m_mNodes.end())
    {
        l_iStartNodeId = 0;
    }
    else if (!l_Entity.m_mNodes.empty())
    {
        // Find lowest node ID
        l_iStartNodeId = l_Entity.m_mNodes.begin()->first;
        for (const auto& l_Pair : l_Entity.m_mNodes)
        {
            if (l_Pair.first < l_iStartNodeId)
            {
                l_iStartNodeId = l_Pair.first;
            }
        }
    }
    
    if (l_iStartNodeId < 0)
    {
        m_strLastError = "Entity has no nodes: " + e_strEntityId;
        return false;
    }
    
    return NavigateTo(e_strEntityId, l_iStartNodeId);
}

//=============================================================================
// ChooseOption
//=============================================================================
bool cTalkSystemBehaviorController::ChooseOption(int e_iOptionIndex)
{
    if (!m_State.m_bIsActive)
    {
        m_strLastError = "No active conversation";
        return false;
    }
    
    if (e_iOptionIndex < 0 || e_iOptionIndex >= static_cast<int>(m_State.m_vResolvedOptions.size()))
    {
        m_strLastError = "Invalid option index";
        return false;
    }
    
    const sDialogueOption& l_ChosenOption = m_State.m_vResolvedOptions[e_iOptionIndex];
    
    // Apply effects (set/clear flags, execute actions)
    ApplyOptionEffects(l_ChosenOption);
    
    // Determine next destination
    std::string l_strNextEntityId = l_ChosenOption.m_strNextEntityId.empty() 
        ? m_State.m_strCurrentEntityId 
        : l_ChosenOption.m_strNextEntityId;
    int l_iNextNodeId = l_ChosenOption.m_iNextNodeId;
    
    // Check for end of conversation
    if (l_iNextNodeId == -1)
    {
        EndConversation();
        return false; // Conversation ended
    }
    
    // Navigate to next node
    return NavigateTo(l_strNextEntityId, l_iNextNodeId);
}

//=============================================================================
// EndConversation
//=============================================================================
void cTalkSystemBehaviorController::EndConversation()
{
    m_State.m_bIsActive = false;
    m_State.m_strCurrentEntityId.clear();
    m_State.m_iCurrentNodeId = -1;
    m_State.m_strResolvedTextKey.clear();
    m_State.m_strResolvedSpeaker.clear();
    m_State.m_vResolvedOptions.clear();
}

//=============================================================================
// GetCurrentText
//=============================================================================
std::string cTalkSystemBehaviorController::GetCurrentText() const
{
    return GetLocalizedString(m_State.m_strResolvedTextKey);
}

//=============================================================================
// GetOptionText
//=============================================================================
std::string cTalkSystemBehaviorController::GetOptionText(int e_iOptionIndex) const
{
    if (e_iOptionIndex < 0 || e_iOptionIndex >= static_cast<int>(m_State.m_vResolvedOptions.size()))
    {
        return "";
    }
    return GetLocalizedString(m_State.m_vResolvedOptions[e_iOptionIndex].m_strLabelKey);
}

//=============================================================================
// HasFlag
//=============================================================================
bool cTalkSystemBehaviorController::HasFlag(const std::string& e_strFlag) const
{
    // Check internal flags first
    if (m_setFlags.find(e_strFlag) != m_setFlags.end())
    {
        return true;
    }
    
    // Check external flag source if callback is set
    if (m_ExternalFlagCheckCallback)
    {
        return m_ExternalFlagCheckCallback(e_strFlag);
    }
    
    return false;
}

//=============================================================================
// SetFlag
//=============================================================================
void cTalkSystemBehaviorController::SetFlag(const std::string& e_strFlag)
{
    if (!e_strFlag.empty())
    {
        m_setFlags.insert(e_strFlag);
    }
}

//=============================================================================
// ClearFlag
//=============================================================================
void cTalkSystemBehaviorController::ClearFlag(const std::string& e_strFlag)
{
    m_setFlags.erase(e_strFlag);
}

//=============================================================================
// GetSequenceIndex
//=============================================================================
int cTalkSystemBehaviorController::GetSequenceIndex(const std::string& e_strSequenceFlag) const
{
    auto l_It = m_mSequenceIndices.find(e_strSequenceFlag);
    if (l_It != m_mSequenceIndices.end())
    {
        return l_It->second;
    }
    return 0;
}

//=============================================================================
// SetSequenceIndex
//=============================================================================
void cTalkSystemBehaviorController::SetSequenceIndex(const std::string& e_strSequenceFlag, int e_iIndex)
{
    m_mSequenceIndices[e_strSequenceFlag] = e_iIndex;
}

//=============================================================================
// GetEntityAlias
//=============================================================================
std::string cTalkSystemBehaviorController::GetEntityAlias(const std::string& e_strEntityId) const
{
    auto l_It = m_DialogueData.m_mEntities.find(e_strEntityId);
    if (l_It != m_DialogueData.m_mEntities.end())
    {
        return l_It->second.m_strAlias;
    }
    return "";
}

//=============================================================================
// HasEntity
//=============================================================================
bool cTalkSystemBehaviorController::HasEntity(const std::string& e_strEntityId) const
{
    return m_DialogueData.m_mEntities.find(e_strEntityId) != m_DialogueData.m_mEntities.end();
}

//=============================================================================
// NavigateTo
//=============================================================================
bool cTalkSystemBehaviorController::NavigateTo(const std::string& e_strEntityId, int e_iNodeId)
{
    // Find entity
    auto l_EntityIt = m_DialogueData.m_mEntities.find(e_strEntityId);
    if (l_EntityIt == m_DialogueData.m_mEntities.end())
    {
        m_strLastError = "Entity not found: " + e_strEntityId;
        EndConversation();
        return false;
    }
    
    // Find node
    const sDialogueEntity& l_Entity = l_EntityIt->second;
    auto l_NodeIt = l_Entity.m_mNodes.find(e_iNodeId);
    if (l_NodeIt == l_Entity.m_mNodes.end())
    {
        m_strLastError = "Node not found: " + std::to_string(e_iNodeId) + " in entity " + e_strEntityId;
        EndConversation();
        return false;
    }
    
    // Update state
    m_State.m_strCurrentEntityId = e_strEntityId;
    m_State.m_iCurrentNodeId = e_iNodeId;
    m_State.m_bIsActive = true;
    
    // Resolve the node content
    return ResolveCurrentNode();
}

//=============================================================================
// ResolveCurrentNode
//=============================================================================
bool cTalkSystemBehaviorController::ResolveCurrentNode()
{
    auto l_EntityIt = m_DialogueData.m_mEntities.find(m_State.m_strCurrentEntityId);
    if (l_EntityIt == m_DialogueData.m_mEntities.end())
    {
        return false;
    }
    
    auto l_NodeIt = l_EntityIt->second.m_mNodes.find(m_State.m_iCurrentNodeId);
    if (l_NodeIt == l_EntityIt->second.m_mNodes.end())
    {
        return false;
    }
    
    const sDialogueNode& l_Node = l_NodeIt->second;
    
    // Set speaker
    m_State.m_strResolvedSpeaker = l_Node.m_strSpeaker;
    
    // Resolve based on node type
    switch (l_Node.m_eType)
    {
    case eDialogueNodeType::Normal:
        m_State.m_strResolvedTextKey = l_Node.m_strTextKey;
        m_State.m_vResolvedOptions = l_Node.m_vOptions;
        break;
        
    case eDialogueNodeType::Conditional:
        {
            // Find first matching branch
            bool l_bFound = false;
            for (const auto& l_Branch : l_Node.m_vBranches)
            {
                if (CheckConditions(l_Branch.m_vConditions))
                {
                    m_State.m_strResolvedTextKey = l_Branch.m_strTextKey;
                    m_State.m_vResolvedOptions = l_Branch.m_vOptions;
                    l_bFound = true;
                    break;
                }
            }
            if (!l_bFound)
            {
                m_strLastError = "No matching branch in conditional node";
                return false;
            }
        }
        break;
        
    case eDialogueNodeType::Sequence:
        {
            if (l_Node.m_vContents.empty())
            {
                m_strLastError = "Empty sequence node";
                return false;
            }
            
            int l_iIndex = GetSequenceIndex(l_Node.m_strSequenceFlag);
            int l_iContentCount = static_cast<int>(l_Node.m_vContents.size());
            
            // Clamp index
            if (l_iIndex >= l_iContentCount)
            {
                l_iIndex = l_Node.m_bLoop ? 0 : (l_iContentCount - 1);
            }
            
            const sDialogueContent& l_Content = l_Node.m_vContents[l_iIndex];
            m_State.m_strResolvedTextKey = l_Content.m_strTextKey;
            m_State.m_vResolvedOptions = l_Content.m_vOptions;
            
            // Advance sequence index for next time
            int l_iNextIndex = l_iIndex + 1;
            if (l_iNextIndex >= l_iContentCount)
            {
                l_iNextIndex = l_Node.m_bLoop ? 0 : l_iContentCount - 1;
            }
            SetSequenceIndex(l_Node.m_strSequenceFlag, l_iNextIndex);
        }
        break;
        
    case eDialogueNodeType::Random:
        {
            if (l_Node.m_vContents.empty())
            {
                m_strLastError = "Empty random node";
                return false;
            }
            
            std::uniform_int_distribution<int> l_Dist(0, static_cast<int>(l_Node.m_vContents.size()) - 1);
            int l_iIndex = l_Dist(m_RandomEngine);
            
            const sDialogueContent& l_Content = l_Node.m_vContents[l_iIndex];
            m_State.m_strResolvedTextKey = l_Content.m_strTextKey;
            m_State.m_vResolvedOptions = l_Content.m_vOptions;
        }
        break;
        
    case eDialogueNodeType::Once:
        {
            bool l_bSeen = HasFlag(l_Node.m_strOnceFlag);
            
            if (!l_bSeen)
            {
                m_State.m_strResolvedTextKey = l_Node.m_FirstTime.m_strTextKey;
                m_State.m_vResolvedOptions = l_Node.m_FirstTime.m_vOptions;
                
                // Set the once flag
                SetFlag(l_Node.m_strOnceFlag);
                
                // Apply any setFlags from firstTime
                for (const auto& l_strFlag : l_Node.m_FirstTime.m_vSetFlags)
                {
                    SetFlag(l_strFlag);
                }
            }
            else
            {
                m_State.m_strResolvedTextKey = l_Node.m_Repeat.m_strTextKey;
                m_State.m_vResolvedOptions = l_Node.m_Repeat.m_vOptions;
            }
        }
        break;
        
    case eDialogueNodeType::Check:
        {
            bool l_bPass = PerformCheck(l_Node.m_strCheckType, l_Node.m_strCheckParam);
            
            if (l_bPass)
            {
                m_State.m_strResolvedTextKey = l_Node.m_Pass.m_strTextKey;
                m_State.m_vResolvedOptions = l_Node.m_Pass.m_vOptions;
            }
            else
            {
                m_State.m_strResolvedTextKey = l_Node.m_Fail.m_strTextKey;
                m_State.m_vResolvedOptions = l_Node.m_Fail.m_vOptions;
            }
        }
        break;
    }
    
    return true;
}

//=============================================================================
// CheckConditions
//=============================================================================
bool cTalkSystemBehaviorController::CheckConditions(const std::vector<std::string>& e_vConditions) const
{
    // Empty conditions = always true (default/fallback)
    if (e_vConditions.empty())
    {
        return true;
    }
    
    // All conditions must be true (AND logic)
    for (const auto& l_strCondition : e_vConditions)
    {
        if (!CheckSingleCondition(l_strCondition))
        {
            return false;
        }
    }
    
    return true;
}

//=============================================================================
// CheckSingleCondition
//=============================================================================
bool cTalkSystemBehaviorController::CheckSingleCondition(const std::string& e_strCondition) const
{
    if (e_strCondition.empty())
    {
        return true;
    }
    
    // Check for negation prefix "!"
    if (e_strCondition[0] == '!')
    {
        std::string l_strFlag = e_strCondition.substr(1);
        return !HasFlag(l_strFlag);
    }
    
    return HasFlag(e_strCondition);
}

//=============================================================================
// PerformCheck
//=============================================================================
bool cTalkSystemBehaviorController::PerformCheck(const std::string& e_strCheckType, const std::string& e_strCheckParam) const
{
    if (e_strCheckType == "HasItem")
    {
        if (m_ItemCheckCallback)
        {
            return m_ItemCheckCallback(e_strCheckType, e_strCheckParam);
        }
        // Fallback: check as a flag
        return HasFlag("item_" + e_strCheckParam);
    }
    else if (e_strCheckType == "HasQuest")
    {
        if (m_ItemCheckCallback)
        {
            return m_ItemCheckCallback(e_strCheckType, e_strCheckParam);
        }
        return HasFlag("quest_" + e_strCheckParam);
    }
    else if (e_strCheckType == "HasFlag")
    {
        return HasFlag(e_strCheckParam);
    }
    
    // Unknown check type - use callback if available
    if (m_ItemCheckCallback)
    {
        return m_ItemCheckCallback(e_strCheckType, e_strCheckParam);
    }
    
    return false;
}

//=============================================================================
// ApplyOptionEffects
//=============================================================================
void cTalkSystemBehaviorController::ApplyOptionEffects(const sDialogueOption& e_Option)
{
    // Set flags
    for (const auto& l_strFlag : e_Option.m_vSetFlags)
    {
        SetFlag(l_strFlag);
    }
    
    // Clear flags
    for (const auto& l_strFlag : e_Option.m_vClearFlags)
    {
        ClearFlag(l_strFlag);
    }
    
    // Execute actions
    if (m_ActionCallback)
    {
        for (const auto& l_strAction : e_Option.m_vActions)
        {
            m_ActionCallback(l_strAction);
        }
    }
}

//=============================================================================
// GetLocalizedString
//=============================================================================
std::string cTalkSystemBehaviorController::GetLocalizedString(const std::string& e_strKey) const
{
    if (e_strKey.empty())
    {
        return "";
    }
    
    // Try current language
    auto l_LangIt = m_LocalizationData.m_mLanguages.find(m_strCurrentLanguage);
    if (l_LangIt != m_LocalizationData.m_mLanguages.end())
    {
        auto l_StrIt = l_LangIt->second.find(e_strKey);
        if (l_StrIt != l_LangIt->second.end())
        {
            return l_StrIt->second;
        }
    }
    
    // Fallback to English
    if (m_strCurrentLanguage != "en")
    {
        l_LangIt = m_LocalizationData.m_mLanguages.find("en");
        if (l_LangIt != m_LocalizationData.m_mLanguages.end())
        {
            auto l_StrIt = l_LangIt->second.find(e_strKey);
            if (l_StrIt != l_LangIt->second.end())
            {
                return l_StrIt->second;
            }
        }
    }
    
    // Return key itself as fallback (helps debug missing translations)
    return "[" + e_strKey + "]";
}
