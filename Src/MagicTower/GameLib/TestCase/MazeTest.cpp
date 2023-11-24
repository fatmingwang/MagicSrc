#include "stdafx.h"
#include "MazeTest.h"
#include "../Level/MazeRender.h"
#include "../GameCamera/GameCamera.h"
TYPDE_DEFINE_MARCO(cMazeTest);
TYPDE_DEFINE_MARCO(cTweenTest);

cMazeTest::cMazeTest()
{
	m_pCyucelenMazeGrid = nullptr;
	this->SetName(cMazeTest::TypeID);
}

cMazeTest::~cMazeTest()
{
	SAFE_DELETE(m_pCyucelenMazeGrid);
}

void cMazeTest::Init()
{
	m_pCyucelenMazeGrid = new cMazeRender(6, 6);
	m_pCyucelenMazeGrid->SetLocalPosition(Vector3(150, 150, 0));
	//g_pCyucelenMazeGrid->GenRandomMap(true,0.9);
	m_pCyucelenMazeGrid->GenRandomMap();
}

void cMazeTest::Update(float e_fElpaseTime)
{
}

void cMazeTest::Render()
{
	cGameCamera::GetInstance()->Render();
	m_pCyucelenMazeGrid->Render();
	m_pCyucelenMazeGrid->DebugRender(true);
}

void cMazeTest::KeyDown(char e_cKey)
{
	m_pCyucelenMazeGrid->KeyUp(e_cKey);
	cGameCamera::GetInstance()->SetCurrentPos(m_pCyucelenMazeGrid->GetCurrentPos());
}

cTweenTest::cTweenTest()
{
	this->SetName(cTweenTest::TypeID);
	m_pTweenyTestObject = nullptr;
}

cTweenTest::~cTweenTest()
{
	SAFE_DELETE(m_pTweenyTestObject);
}

void cTweenTest::Init()
{
	m_pTweenyTestObject = new cTweenyTestObject();
	//m_pTweenyTestObject->KeyUp();
}

void cTweenTest::Update(float e_fElpaseTime)
{
}

void cTweenTest::Render()
{
	//cGameCamera::GetInstance()->Render();
	glEnable2D(3940, 2160);
	RenderFilledRectangle(Vector2::Zero, 1920 * 2, 1080 * 2, Vector4(0.1f, 0.1f, 0.1f, 0.7f), 0);
	int l_iIndex = 1;
	for (auto l_vPos : m_pTweenyTestObject->m_vTestVector)
	{
		//cGameApp::RenderFont(50.f, l_vPos.y, ValueToString((tweeny::easing::enumerated)l_iIndex));
		GLRender::RenderSphere(l_vPos, 30);
		++l_iIndex;
	}
	l_iIndex = 1;
	for (auto l_vPos : m_pTweenyTestObject->m_vTestVector)
	{
		cGameApp::RenderFont(50.f, l_vPos.y, ValueToString((tweeny::easing::enumerated)l_iIndex));
		//GLRender::RenderSphere(l_vPos, 30);
		++l_iIndex;
	}
	if (m_pTweenyTestObject->m_vTestVector.size() == 0)
	{
		cGameApp::RenderFont(Vector2(1920, 1080), L"please press any key");
	}
}

void cTweenTest::KeyDown(char e_cKey)
{
	m_pTweenyTestObject->KeyUp();
}

void AddTestPhase(eTestPhase e_eTestPhase, cPhaseManager& e_PhaseManager)
{
	switch (e_eTestPhase)
	{
		case eTestPhase::eTP_MAZE:
			if (!e_PhaseManager.GetObject(cMazeTest::TypeID))
			{
				cMazeTest*l_pMazeTest = new cMazeTest();
				e_PhaseManager.AddObject(l_pMazeTest);
			}
		break;
		case eTestPhase::eTP_TWEEN:
			if (!e_PhaseManager.GetObject(cTweenTest::TypeID))
			{
				cTweenTest*l_pTweenTest = new cTweenTest();
				e_PhaseManager.AddObject(l_pTweenTest);
			}
		break;
	}
}

void SetWorkingTestPhase(eTestPhase e_eTestPhase,cPhaseManager& e_PhaseManager)
{
	switch (e_eTestPhase)
	{
	case eTestPhase::eTP_MAZE:
		e_PhaseManager.SetCurrentCurrentPhase(cMazeTest::TypeID);
		break;
	case eTestPhase::eTP_TWEEN:
		e_PhaseManager.SetCurrentCurrentPhase(cTweenTest::TypeID);
		break;
	}
}