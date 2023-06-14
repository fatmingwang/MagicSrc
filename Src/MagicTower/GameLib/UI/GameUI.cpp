#include "stdafx.h"
#include "../FileNameDefine.h"
#include "FingerMovingDirection.h"
#include "GameUI.h"
#include "../FileNameDefine.h"



TYPDE_DEFINE_MARCO(cGameUI)
cGameUI::cGameUI()
{
	m_pScoreNumberImage = nullptr;
	cFingerMovingDirection*l_pFingerMovingDirection = new cFingerMovingDirection();
	this->AddObject(l_pFingerMovingDirection);
}

cGameUI::~cGameUI()
{
	SAFE_DELETE(m_pScoreNumberImage);
}

void cGameUI::Init()
{
	if (!m_pScoreNumberImage)
	{
		//cPuzzleImage* l_pNumber = cGameApp::GetPuzzleImageByFileName(PLAYER_NUMRIAL);
		//m_pScoreNumberImage = l_pNumber->GetNumeralImageByName(L"PlayerScoreNumerial0", L"PlayerScoreNumerial9");
		//m_pScoreNumberImage->SetLocalPosition(Vector3(350, 20, 0));
		//m_pScoreNumberImage->SetValue(100);
	}
}

void cGameUI::Update(float e_fElpaseTime)
{
	cClickBehaviorDispatcher::Update(e_fElpaseTime);
}

void cGameUI::Render()
{
	glEnable2D(720, 1280);
	if (m_pScoreNumberImage)
	{
		//cOpenGLRender::DoBatchDataMultiTextureStart();
		//m_pScoreNumberImage->SetLocalPosition(Vector3(350, 220, 0));
		//m_pScoreNumberImage->SetValue(100);
		//m_pScoreNumberImage->Render();
		//m_pScoreNumberImage->SetLocalPosition(Vector3(350, 420, 0));
		//m_pScoreNumberImage->SetValue(900);
		m_pScoreNumberImage->Render();
		//cOpenGLRender::DoBatchDataMultiTextureEnd();
	}
}
