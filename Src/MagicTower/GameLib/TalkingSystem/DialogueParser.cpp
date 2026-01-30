#include "DialogueParser.h"
#include "json.hpp"
#include <fstream>
#include <sstream>

using json = nlohmann::json;

//=============================================================================
// ParseDialogueFile
//=============================================================================
bool cDialogueParser::ParseDialogueFile(const std::string& e_strFilePath, sDialogueData& e_OutData)
{
    std::ifstream l_File(e_strFilePath);
    if (!l_File.is_open())
    {
        m_strLastError = "Failed to open file: " + e_strFilePath;
        return false;
    }
    
    std::stringstream l_Buffer;
    l_Buffer << l_File.rdbuf();
    return ParseDialogueString(l_Buffer.str(), e_OutData);
}

//=============================================================================
// ParseDialogueString
//=============================================================================
bool cDialogueParser::ParseDialogueString(const std::string& e_strJsonContent, sDialogueData& e_OutData)
{
    try
    {
        json l_Root = json::parse(e_strJsonContent);
        
        // Parse version
        if (l_Root.contains("version"))
        {
            e_OutData.m_strVersion = l_Root["version"].get<std::string>();
        }
        
        // Parse entities
        if (!l_Root.contains("entities"))
        {
            m_strLastError = "Missing 'entities' field in dialogue JSON";
            return false;
        }
        
        const json& l_Entities = l_Root["entities"];
        for (auto it = l_Entities.begin(); it != l_Entities.end(); ++it)
        {
            std::string l_strEntityId = it.key();
            const json& l_EntityJson = it.value();
            
            sDialogueEntity l_Entity;
            l_Entity.m_strEntityId = l_strEntityId;
            
            // Parse alias
            if (l_EntityJson.contains("alias"))
            {
                l_Entity.m_strAlias = l_EntityJson["alias"].get<std::string>();
            }
            
            // Parse nodes
            if (l_EntityJson.contains("nodes"))
            {
                for (const auto& l_NodeJson : l_EntityJson["nodes"])
                {
                    sDialogueNode l_Node;
                    ParseNode(&l_NodeJson, l_Node);
                    l_Entity.m_mNodes[l_Node.m_iNodeId] = std::move(l_Node);
                }
            }
            
            e_OutData.m_mEntities[l_strEntityId] = std::move(l_Entity);
        }
        
        return true;
    }
    catch (const json::exception& e)
    {
        m_strLastError = std::string("JSON parse error: ") + e.what();
        return false;
    }
}

//=============================================================================
// ParseLocalizationFile
//=============================================================================
bool cDialogueParser::ParseLocalizationFile(const std::string& e_strFilePath, sLocalizationData& e_OutData)
{
    std::ifstream l_File(e_strFilePath);
    if (!l_File.is_open())
    {
        m_strLastError = "Failed to open file: " + e_strFilePath;
        return false;
    }
    
    std::stringstream l_Buffer;
    l_Buffer << l_File.rdbuf();
    return ParseLocalizationString(l_Buffer.str(), e_OutData);
}

//=============================================================================
// ParseLocalizationString
//=============================================================================
bool cDialogueParser::ParseLocalizationString(const std::string& e_strJsonContent, sLocalizationData& e_OutData)
{
    try
    {
        json l_Root = json::parse(e_strJsonContent);
        
        // Parse version
        if (l_Root.contains("version"))
        {
            e_OutData.m_strVersion = l_Root["version"].get<std::string>();
        }
        
        // Parse languages
        if (!l_Root.contains("languages"))
        {
            m_strLastError = "Missing 'languages' field in localization JSON";
            return false;
        }
        
        const json& l_Languages = l_Root["languages"];
        for (auto it = l_Languages.begin(); it != l_Languages.end(); ++it)
        {
            std::string l_strLangCode = it.key();
            const json& l_LangData = it.value();
            
            std::unordered_map<std::string, std::string> l_mStrings;
            for (auto strIt = l_LangData.begin(); strIt != l_LangData.end(); ++strIt)
            {
                if (strIt.key() != "_comment" && strIt.value().is_string())
                {
                    l_mStrings[strIt.key()] = strIt.value().get<std::string>();
                }
            }
            
            e_OutData.m_mLanguages[l_strLangCode] = std::move(l_mStrings);
        }
        
        return true;
    }
    catch (const json::exception& e)
    {
        m_strLastError = std::string("JSON parse error: ") + e.what();
        return false;
    }
}

//=============================================================================
// ParseNodeType
//=============================================================================
eDialogueNodeType cDialogueParser::ParseNodeType(const std::string& e_strType)
{
    if (e_strType == "conditional") return eDialogueNodeType::Conditional;
    if (e_strType == "sequence")    return eDialogueNodeType::Sequence;
    if (e_strType == "random")      return eDialogueNodeType::Random;
    if (e_strType == "once")        return eDialogueNodeType::Once;
    if (e_strType == "check")       return eDialogueNodeType::Check;
    return eDialogueNodeType::Normal;
}

//=============================================================================
// ParseStringArray
//=============================================================================
void cDialogueParser::ParseStringArray(const void* e_pJsonArray, std::vector<std::string>& e_OutStrings)
{
    const json& l_Array = *static_cast<const json*>(e_pJsonArray);
    e_OutStrings.clear();
    for (const auto& l_Item : l_Array)
    {
        if (l_Item.is_string())
        {
            e_OutStrings.push_back(l_Item.get<std::string>());
        }
    }
}

//=============================================================================
// ParseOptions
//=============================================================================
void cDialogueParser::ParseOptions(const void* e_pJsonArray, std::vector<sDialogueOption>& e_OutOptions)
{
    const json& l_Array = *static_cast<const json*>(e_pJsonArray);
    e_OutOptions.clear();
    
    for (const auto& l_OptJson : l_Array)
    {
        sDialogueOption l_Opt;
        
        if (l_OptJson.contains("labelKey"))
        {
            l_Opt.m_strLabelKey = l_OptJson["labelKey"].get<std::string>();
        }
        if (l_OptJson.contains("nextNodeId"))
        {
            l_Opt.m_iNextNodeId = l_OptJson["nextNodeId"].get<int>();
        }
        if (l_OptJson.contains("nextEntityId"))
        {
            l_Opt.m_strNextEntityId = l_OptJson["nextEntityId"].get<std::string>();
        }
        if (l_OptJson.contains("setFlags"))
        {
            ParseStringArray(&l_OptJson["setFlags"], l_Opt.m_vSetFlags);
        }
        if (l_OptJson.contains("clearFlags"))
        {
            ParseStringArray(&l_OptJson["clearFlags"], l_Opt.m_vClearFlags);
        }
        if (l_OptJson.contains("actions"))
        {
            ParseStringArray(&l_OptJson["actions"], l_Opt.m_vActions);
        }
        
        e_OutOptions.push_back(std::move(l_Opt));
    }
}

//=============================================================================
// ParseContent
//=============================================================================
void cDialogueParser::ParseContent(const void* e_pJsonObj, sDialogueContent& e_OutContent)
{
    const json& l_Obj = *static_cast<const json*>(e_pJsonObj);
    
    if (l_Obj.contains("textKey"))
    {
        e_OutContent.m_strTextKey = l_Obj["textKey"].get<std::string>();
    }
    if (l_Obj.contains("options"))
    {
        ParseOptions(&l_Obj["options"], e_OutContent.m_vOptions);
    }
    if (l_Obj.contains("setFlags"))
    {
        ParseStringArray(&l_Obj["setFlags"], e_OutContent.m_vSetFlags);
    }
}

//=============================================================================
// ParseBranch
//=============================================================================
void cDialogueParser::ParseBranch(const void* e_pJsonObj, sDialogueBranch& e_OutBranch)
{
    const json& l_Obj = *static_cast<const json*>(e_pJsonObj);
    
    if (l_Obj.contains("conditions"))
    {
        ParseStringArray(&l_Obj["conditions"], e_OutBranch.m_vConditions);
    }
    if (l_Obj.contains("textKey"))
    {
        e_OutBranch.m_strTextKey = l_Obj["textKey"].get<std::string>();
    }
    if (l_Obj.contains("options"))
    {
        ParseOptions(&l_Obj["options"], e_OutBranch.m_vOptions);
    }
}

//=============================================================================
// ParseNode
//=============================================================================
void cDialogueParser::ParseNode(const void* e_pJsonObj, sDialogueNode& e_OutNode)
{
    const json& l_Obj = *static_cast<const json*>(e_pJsonObj);
    
    // Common fields
    if (l_Obj.contains("nodeId"))
    {
        e_OutNode.m_iNodeId = l_Obj["nodeId"].get<int>();
    }
    if (l_Obj.contains("type"))
    {
        e_OutNode.m_eType = ParseNodeType(l_Obj["type"].get<std::string>());
    }
    if (l_Obj.contains("speaker"))
    {
        e_OutNode.m_strSpeaker = l_Obj["speaker"].get<std::string>();
    }
    
    // Type-specific fields
    switch (e_OutNode.m_eType)
    {
    case eDialogueNodeType::Normal:
        if (l_Obj.contains("textKey"))
        {
            e_OutNode.m_strTextKey = l_Obj["textKey"].get<std::string>();
        }
        if (l_Obj.contains("options"))
        {
            ParseOptions(&l_Obj["options"], e_OutNode.m_vOptions);
        }
        break;
        
    case eDialogueNodeType::Conditional:
        if (l_Obj.contains("branches"))
        {
            for (const auto& l_BranchJson : l_Obj["branches"])
            {
                sDialogueBranch l_Branch;
                ParseBranch(&l_BranchJson, l_Branch);
                e_OutNode.m_vBranches.push_back(std::move(l_Branch));
            }
        }
        break;
        
    case eDialogueNodeType::Sequence:
        if (l_Obj.contains("sequenceFlag"))
        {
            e_OutNode.m_strSequenceFlag = l_Obj["sequenceFlag"].get<std::string>();
        }
        if (l_Obj.contains("loop"))
        {
            e_OutNode.m_bLoop = l_Obj["loop"].get<bool>();
        }
        if (l_Obj.contains("contents"))
        {
            for (const auto& l_ContentJson : l_Obj["contents"])
            {
                sDialogueContent l_Content;
                ParseContent(&l_ContentJson, l_Content);
                e_OutNode.m_vContents.push_back(std::move(l_Content));
            }
        }
        break;
        
    case eDialogueNodeType::Random:
        if (l_Obj.contains("contents"))
        {
            for (const auto& l_ContentJson : l_Obj["contents"])
            {
                sDialogueContent l_Content;
                ParseContent(&l_ContentJson, l_Content);
                e_OutNode.m_vContents.push_back(std::move(l_Content));
            }
        }
        break;
        
    case eDialogueNodeType::Once:
        if (l_Obj.contains("onceFlag"))
        {
            e_OutNode.m_strOnceFlag = l_Obj["onceFlag"].get<std::string>();
        }
        if (l_Obj.contains("firstTime"))
        {
            ParseContent(&l_Obj["firstTime"], e_OutNode.m_FirstTime);
        }
        if (l_Obj.contains("repeat"))
        {
            ParseContent(&l_Obj["repeat"], e_OutNode.m_Repeat);
        }
        break;
        
    case eDialogueNodeType::Check:
        if (l_Obj.contains("checkType"))
        {
            e_OutNode.m_strCheckType = l_Obj["checkType"].get<std::string>();
        }
        if (l_Obj.contains("checkParam"))
        {
            e_OutNode.m_strCheckParam = l_Obj["checkParam"].get<std::string>();
        }
        if (l_Obj.contains("pass"))
        {
            ParseContent(&l_Obj["pass"], e_OutNode.m_Pass);
        }
        if (l_Obj.contains("fail"))
        {
            ParseContent(&l_Obj["fail"], e_OutNode.m_Fail);
        }
        break;
    }
}
