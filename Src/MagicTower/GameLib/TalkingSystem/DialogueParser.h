#pragma once

#include "TalkingSystemData.h"
#include <string>

//=============================================================================
// cDialogueParser - parses dialogue and localization JSON files
//=============================================================================
class cDialogueParser
{
public:
    cDialogueParser() = default;
    ~cDialogueParser() = default;

    // Parse dialogue structure from JSON file
    // Returns true on success
    bool ParseDialogueFile(const std::string& e_strFilePath, sDialogueData& e_OutData);
    
    // Parse dialogue structure from JSON string
    bool ParseDialogueString(const std::string& e_strJsonContent, sDialogueData& e_OutData);
    
    // Parse localization from JSON file
    bool ParseLocalizationFile(const std::string& e_strFilePath, sLocalizationData& e_OutData);
    
    // Parse localization from JSON string
    bool ParseLocalizationString(const std::string& e_strJsonContent, sLocalizationData& e_OutData);

    // Get last error message
    const std::string& GetLastError() const { return m_strLastError; }

private:
    std::string m_strLastError;
    
    // Helper functions
    eDialogueNodeType ParseNodeType(const std::string& e_strType);
    void ParseOptions(const void* e_pJsonArray, std::vector<sDialogueOption>& e_OutOptions);
    void ParseContent(const void* e_pJsonObj, sDialogueContent& e_OutContent);
    void ParseBranch(const void* e_pJsonObj, sDialogueBranch& e_OutBranch);
    void ParseNode(const void* e_pJsonObj, sDialogueNode& e_OutNode);
    void ParseStringArray(const void* e_pJsonArray, std::vector<std::string>& e_OutStrings);
};
