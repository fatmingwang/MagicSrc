#include "stdafx.h"
#include "DialogueUI.h"
#include "../UI/NineGridFrame.h"

// Forward declare or include your font rendering system
// Placeholder functions - replace with your actual implementation
namespace
{
    void RenderText(float e_fX, float e_fY, const Vector4& e_vColor, const char* e_strText)
    {
        // TODO: Replace with your actual text rendering
        // cGameApp::RenderFont(Vector2(e_fX, e_fY), e_vColor, e_strText);
        
        // For now, use ImGui if available
        #ifdef IMGUI_VERSION
        ImGui::SetNextWindowPos(ImVec2(e_fX, e_fY), ImGuiCond_Always);
        ImGui::SetNextWindowBgAlpha(0.0f);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(e_vColor.x, e_vColor.y, e_vColor.z, e_vColor.w));
        ImGui::Begin("##DialogText", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::Text("%s", e_strText);
        ImGui::End();
        ImGui::PopStyleColor();
        #endif
    }
    
    void RenderFilledRect(float e_fX, float e_fY, float e_fWidth, float e_fHeight, const Vector4& e_vColor)
    {
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(e_vColor.x, e_vColor.y, e_vColor.z, e_vColor.w);
        
        glBegin(GL_QUADS);
        glVertex2f(e_fX, e_fY);
        glVertex2f(e_fX + e_fWidth, e_fY);
        glVertex2f(e_fX + e_fWidth, e_fY + e_fHeight);
        glVertex2f(e_fX, e_fY + e_fHeight);
        glEnd();
        
        glColor4f(1.f, 1.f, 1.f, 1.f);
        glDisable(GL_BLEND);
    }
}

//=============================================================================
// Constructor / Destructor
//=============================================================================
cDialogueUI::cDialogueUI()
    : m_pTalkController(nullptr)
    , m_pFrame(nullptr)
    , m_bVisible(false)
    , m_iSelectedOption(0)
    , m_bTypewriterFinished(true)
    , m_fTypewriterProgress(0.f)
{
}

cDialogueUI::~cDialogueUI()
{
    SAFE_DELETE(m_pFrame);
}

//=============================================================================
// Init
//=============================================================================
bool cDialogueUI::Init(const char* e_strFrameImage, float* e_pfNineUV)
{
    SAFE_DELETE(m_pFrame);
    
    if (e_strFrameImage && e_pfNineUV)
    {
        m_pFrame = new cNineGridFrame(e_strFrameImage, e_pfNineUV);
    }
    else
    {
        // Create debug frame if no image provided
        Vector2 l_vCenterSize(50.f, 50.f);
        Vector2 l_vCornerSize(20.f, 20.f);
        Vector2 l_vContentSize(m_Config.m_fFrameWidth, m_Config.m_fFrameHeight);
        m_pFrame = cNineGridFrame::GenerateDebugObject(l_vCenterSize, l_vCornerSize, l_vContentSize);
    }
    
    if (m_pFrame)
    {
        m_pFrame->SetContentSize(Vector2(m_Config.m_fFrameWidth, m_Config.m_fFrameHeight));
    }
    
    return m_pFrame != nullptr;
}

//=============================================================================
// InitWithPuzzleImage
//=============================================================================
bool cDialogueUI::InitWithPuzzleImage(cPuzzleImageUnit* e_pLeftUp, cPuzzleImageUnit* e_pRightBottom)
{
    SAFE_DELETE(m_pFrame);
    
    if (e_pLeftUp && e_pRightBottom)
    {
        m_pFrame = new cNineGridFrame(e_pLeftUp, e_pRightBottom);
        m_pFrame->SetContentSize(Vector2(m_Config.m_fFrameWidth, m_Config.m_fFrameHeight));
        return true;
    }
    
    return false;
}

//=============================================================================
// SetTalkController
//=============================================================================
void cDialogueUI::SetTalkController(cTalkSystemBehaviorController* e_pController)
{
    m_pTalkController = e_pController;
}

//=============================================================================
// Show / Hide
//=============================================================================
void cDialogueUI::Show()
{
    m_bVisible = true;
    m_iSelectedOption = 0;
    
    // Reset typewriter
    if (m_pTalkController && m_Config.m_bUseTypewriter)
    {
        m_strFullText = m_pTalkController->GetCurrentText();
        m_fTypewriterProgress = 0.f;
        m_bTypewriterFinished = false;
        m_strDisplayText.clear();
    }
    else
    {
        m_bTypewriterFinished = true;
    }
    
    // Update option rects
    m_vOptionRects.clear();
    if (m_pTalkController)
    {
        int l_iCount = m_pTalkController->GetOptionCount();
        float l_fOptionWidth = m_Config.m_fFrameWidth - m_Config.m_fTextPaddingLeft - m_Config.m_fTextPaddingRight;
        
        for (int i = 0; i < l_iCount; ++i)
        {
            sOptionRect l_Rect;
            l_Rect.m_fX = m_Config.m_fFrameX + m_Config.m_fTextPaddingLeft;
            l_Rect.m_fY = m_Config.m_fFrameY + m_Config.m_fOptionStartY + i * (m_Config.m_fOptionHeight + m_Config.m_fOptionSpacing);
            l_Rect.m_fWidth = l_fOptionWidth;
            l_Rect.m_fHeight = m_Config.m_fOptionHeight;
            m_vOptionRects.push_back(l_Rect);
        }
    }
}

void cDialogueUI::Hide()
{
    m_bVisible = false;
}

//=============================================================================
// Update
//=============================================================================
void cDialogueUI::Update(float e_fElapsedTime)
{
    if (!m_bVisible)
        return;
    
    UpdateTypewriter(e_fElapsedTime);
}

//=============================================================================
// UpdateTypewriter
//=============================================================================
void cDialogueUI::UpdateTypewriter(float e_fElapsedTime)
{
    if (m_bTypewriterFinished || !m_Config.m_bUseTypewriter)
        return;
    
    m_fTypewriterProgress += e_fElapsedTime * m_Config.m_fTypewriterSpeed;
    
    int l_iCharsToShow = static_cast<int>(m_fTypewriterProgress);
    int l_iFullLength = static_cast<int>(m_strFullText.length());
    
    if (l_iCharsToShow >= l_iFullLength)
    {
        m_strDisplayText = m_strFullText;
        m_bTypewriterFinished = true;
    }
    else
    {
        // Handle UTF-8 properly (simplified - just use substring)
        m_strDisplayText = m_strFullText.substr(0, l_iCharsToShow);
    }
}

//=============================================================================
// SkipTypewriter
//=============================================================================
void cDialogueUI::SkipTypewriter()
{
    m_strDisplayText = m_strFullText;
    m_bTypewriterFinished = true;
}

//=============================================================================
// Render
//=============================================================================
void cDialogueUI::Render()
{
    if (!m_bVisible)
        return;
    
    RenderFrame();
    RenderSpeaker();
    RenderDialogueText();
    RenderOptions();
}

//=============================================================================
// RenderFrame
//=============================================================================
void cDialogueUI::RenderFrame()
{
    if (!m_pFrame)
    {
        // Fallback: render a simple colored rectangle
        RenderFilledRect(
            m_Config.m_fFrameX,
            m_Config.m_fFrameY,
            m_Config.m_fFrameWidth,
            m_Config.m_fFrameHeight,
            Vector4(0.1f, 0.1f, 0.3f, 0.9f)
        );
        return;
    }
    
    m_pFrame->SetLocalPosition(Vector3(
        m_Config.m_fFrameX + m_Config.m_fFrameWidth * 0.5f,
        m_Config.m_fFrameY + m_Config.m_fFrameHeight * 0.5f,
        0.f
    ));
    m_pFrame->Render();
}

//=============================================================================
// RenderSpeaker
//=============================================================================
void cDialogueUI::RenderSpeaker()
{
    if (!m_pTalkController)
        return;
    
    const std::string& l_strSpeaker = m_pTalkController->GetCurrentSpeaker();
    if (l_strSpeaker.empty())
        return;
    
    float l_fX = m_Config.m_fFrameX + m_Config.m_fSpeakerOffsetX;
    float l_fY = m_Config.m_fFrameY + m_Config.m_fSpeakerOffsetY;
    
    // Render speaker name
    RenderText(l_fX, l_fY, m_Config.m_vSpeakerColor, l_strSpeaker.c_str());
}

//=============================================================================
// RenderText
//=============================================================================
void cDialogueUI::RenderDialogueText()
{
    if (!m_pTalkController)
        return;
    
    const std::string& l_strText = m_Config.m_bUseTypewriter ? m_strDisplayText : m_pTalkController->GetCurrentText();
    if (l_strText.empty())
        return;
    
    float l_fX = m_Config.m_fFrameX + m_Config.m_fTextPaddingLeft;
    float l_fY = m_Config.m_fFrameY + m_Config.m_fTextPaddingTop;
    
    // Render dialogue text
    RenderText(l_fX, l_fY, m_Config.m_vTextColor, l_strText.c_str());
}

//=============================================================================
// RenderOptions
//=============================================================================
void cDialogueUI::RenderOptions()
{
    if (!m_pTalkController)
        return;
    
    int l_iCount = m_pTalkController->GetOptionCount();
    
    for (int i = 0; i < l_iCount && i < static_cast<int>(m_vOptionRects.size()); ++i)
    {
        const sOptionRect& l_Rect = m_vOptionRects[i];
        std::string l_strOption = m_pTalkController->GetOptionText(i);
        
        // Highlight selected option
        bool l_bSelected = (i == m_iSelectedOption);
        Vector4 l_vColor = l_bSelected ? m_Config.m_vOptionHighlightColor : m_Config.m_vOptionColor;
        
        // Render option background (optional)
        if (l_bSelected)
        {
            RenderFilledRect(
                l_Rect.m_fX - 5.f,
                l_Rect.m_fY,
                l_Rect.m_fWidth + 10.f,
                l_Rect.m_fHeight,
                Vector4(0.3f, 0.3f, 0.5f, 0.5f)
            );
        }
        
        // Add selection indicator
        std::string l_strDisplay = l_bSelected ? "> " + l_strOption : "  " + l_strOption;
        
        // Render option text
        RenderText(
            l_Rect.m_fX,
            l_Rect.m_fY + l_Rect.m_fHeight * 0.3f,
            l_vColor,
            l_strDisplay.c_str()
        );
    }
}

//=============================================================================
// OnClick
//=============================================================================
bool cDialogueUI::OnClick(int e_iX, int e_iY)
{
    if (!m_bVisible || !m_pTalkController)
        return false;
    
    // If typewriter not finished, skip it
    if (!m_bTypewriterFinished)
    {
        SkipTypewriter();
        return true;
    }
    
    // Check if clicked on an option
    int l_iOptionIndex = GetOptionAtPosition(e_iX, e_iY);
    if (l_iOptionIndex >= 0)
    {
        m_iSelectedOption = l_iOptionIndex;
        ConfirmSelection();
        return true;
    }
    
    // Click anywhere else to advance (if only one option or no options)
    if (m_pTalkController->GetOptionCount() <= 1)
    {
        ConfirmSelection();
        return true;
    }
    
    return false;
}

//=============================================================================
// GetOptionAtPosition
//=============================================================================
int cDialogueUI::GetOptionAtPosition(int e_iX, int e_iY) const
{
    float l_fX = static_cast<float>(e_iX);
    float l_fY = static_cast<float>(e_iY);
    
    for (size_t i = 0; i < m_vOptionRects.size(); ++i)
    {
        const sOptionRect& l_Rect = m_vOptionRects[i];
        if (l_fX >= l_Rect.m_fX && l_fX <= l_Rect.m_fX + l_Rect.m_fWidth &&
            l_fY >= l_Rect.m_fY && l_fY <= l_Rect.m_fY + l_Rect.m_fHeight)
        {
            return static_cast<int>(i);
        }
    }
    
    return -1;
}

//=============================================================================
// Navigation
//=============================================================================
void cDialogueUI::SelectNextOption()
{
    if (!m_pTalkController)
        return;
    
    int l_iCount = m_pTalkController->GetOptionCount();
    if (l_iCount > 0)
    {
        m_iSelectedOption = (m_iSelectedOption + 1) % l_iCount;
    }
}

void cDialogueUI::SelectPreviousOption()
{
    if (!m_pTalkController)
        return;
    
    int l_iCount = m_pTalkController->GetOptionCount();
    if (l_iCount > 0)
    {
        m_iSelectedOption = (m_iSelectedOption - 1 + l_iCount) % l_iCount;
    }
}

void cDialogueUI::SetSelectedOption(int e_iIndex)
{
    if (!m_pTalkController)
        return;
    
    int l_iCount = m_pTalkController->GetOptionCount();
    if (e_iIndex >= 0 && e_iIndex < l_iCount)
    {
        m_iSelectedOption = e_iIndex;
    }
}

void cDialogueUI::ConfirmSelection()
{
    if (!m_pTalkController)
        return;
    
    // Choose the selected option
    bool l_bContinues = m_pTalkController->ChooseOption(m_iSelectedOption);
    
    if (l_bContinues)
    {
        // Reset for next node
        m_iSelectedOption = 0;
        Show(); // Re-initialize typewriter and option rects
    }
    else
    {
        // Conversation ended
        Hide();
    }
}
