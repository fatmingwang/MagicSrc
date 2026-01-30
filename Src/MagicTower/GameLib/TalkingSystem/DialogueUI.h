#pragma once

#include "TalkSystemBehaviorController.h"
#include <vector>
#include <string>

// Forward declarations
class cNineGridFrame;
class cBaseImage;
class cPuzzleImageUnit;

// Vector types - adjust based on your engine
#ifndef VECTOR4_DEFINED
struct Vector4
{
    float x, y, z, w;
    Vector4() : x(0), y(0), z(0), w(0) {}
    Vector4(float e_x, float e_y, float e_z, float e_w) : x(e_x), y(e_y), z(e_z), w(e_w) {}
};
#define VECTOR4_DEFINED
#endif

//=============================================================================
// sDialogueUIConfig - Configuration for dialogue UI appearance
//=============================================================================
struct sDialogueUIConfig
{
    // Frame position and size
    float m_fFrameX = 50.f;
    float m_fFrameY = 900.f;
    float m_fFrameWidth = 620.f;
    float m_fFrameHeight = 300.f;
    
    // Text area padding inside frame
    float m_fTextPaddingLeft = 30.f;
    float m_fTextPaddingTop = 20.f;
    float m_fTextPaddingRight = 30.f;
    float m_fTextPaddingBottom = 20.f;
    
    // Speaker name position offset from frame
    float m_fSpeakerOffsetX = 20.f;
    float m_fSpeakerOffsetY = -40.f;
    
    // Option button settings
    float m_fOptionStartY = 180.f;
    float m_fOptionHeight = 40.f;
    float m_fOptionSpacing = 10.f;
    
    // Colors (RGBA 0-1)
    Vector4 m_vTextColor = Vector4(1.f, 1.f, 1.f, 1.f);
    Vector4 m_vSpeakerColor = Vector4(1.f, 0.9f, 0.5f, 1.f);
    Vector4 m_vOptionColor = Vector4(0.9f, 0.9f, 0.9f, 1.f);
    Vector4 m_vOptionHighlightColor = Vector4(1.f, 1.f, 0.5f, 1.f);
    
    // Typewriter effect
    bool m_bUseTypewriter = true;
    float m_fTypewriterSpeed = 30.f; // characters per second
};

//=============================================================================
// cDialogueUI - Renders dialogue with 9-grid frame
//=============================================================================
class cDialogueUI
{
public:
    cDialogueUI();
    ~cDialogueUI();
    
    //=========================================================================
    // Initialization
    //=========================================================================
    
    // Initialize with 9-grid frame images
    // e_strFrameImage: path to 9-grid texture
    // e_pfNineUV: UV coordinates for 9 grid sections (36 floats: 9 sections * 4 UV values)
    bool Init(const char* e_strFrameImage, float* e_pfNineUV = nullptr);
    
    // Initialize with puzzle image units for 9-grid
    bool InitWithPuzzleImage(cPuzzleImageUnit* e_pLeftUp, cPuzzleImageUnit* e_pRightBottom);
    
    // Set the talk system controller to display
    void SetTalkController(cTalkSystemBehaviorController* e_pController);
    
    // Set UI configuration
    void SetConfig(const sDialogueUIConfig& e_Config) { m_Config = e_Config; }
    const sDialogueUIConfig& GetConfig() const { return m_Config; }
    
    //=========================================================================
    // Show/Hide
    //=========================================================================
    
    void Show();
    void Hide();
    bool IsVisible() const { return m_bVisible; }
    
    //=========================================================================
    // Update & Render
    //=========================================================================
    
    void Update(float e_fElapsedTime);
    void Render();
    
    //=========================================================================
    // Input Handling
    //=========================================================================
    
    // Handle click/touch input, returns true if handled
    bool OnClick(int e_iX, int e_iY);
    
    // Keyboard/gamepad navigation
    void SelectNextOption();
    void SelectPreviousOption();
    void ConfirmSelection();
    
    // Get currently selected option index
    int GetSelectedOption() const { return m_iSelectedOption; }
    void SetSelectedOption(int e_iIndex);
    
    //=========================================================================
    // Typewriter Effect
    //=========================================================================
    
    // Skip typewriter animation and show full text
    void SkipTypewriter();
    bool IsTypewriterFinished() const { return m_bTypewriterFinished; }

private:
    //=========================================================================
    // Internal Methods
    //=========================================================================
    
    void RenderFrame();
    void RenderSpeaker();
    void RenderDialogueText();
    void RenderOptions();
    void UpdateTypewriter(float e_fElapsedTime);
    
    // Check if point is inside option button
    int GetOptionAtPosition(int e_iX, int e_iY) const;
    
    //=========================================================================
    // Member Variables
    //=========================================================================
    
    // Reference to talk system (not owned)
    cTalkSystemBehaviorController* m_pTalkController;
    
    // 9-grid frame for dialogue box
    cNineGridFrame* m_pFrame;
    
    // UI state
    bool m_bVisible;
    int m_iSelectedOption;
    
    // Typewriter effect state
    bool m_bTypewriterFinished;
    float m_fTypewriterProgress; // characters shown
    std::string m_strFullText;
    std::string m_strDisplayText;
    
    // Configuration
    sDialogueUIConfig m_Config;
    
    // Cached option rects for click detection
    struct sOptionRect
    {
        float m_fX, m_fY, m_fWidth, m_fHeight;
    };
    std::vector<sOptionRect> m_vOptionRects;
};
