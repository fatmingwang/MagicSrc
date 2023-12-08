#include "stdafx.h"
#include "MazeTest.h"
#include "../Level/MazeRender.h"
#include "../GameCamera/GameCamera.h"
#include "./GamePlayTesting.h"

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
	cGameCamera::DestroyInstance();
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
	SAFE_DELETE(m_pTweenyCurveWithTime);
	cGameCamera::DestroyInstance();
}

void cTweenTest::Init()
{
	//m_pTweenyTestObject = new cTweenyTestObject();
	m_pTweenyCurveWithTime = new cTweenyCurveWithTime();
	cCurveWithTime	l_Data;
	l_Data.AddPoint(Vector3(50, 50, 0), 0);
	l_Data.AddPoint(Vector3(150, 50, 0), 1);
	l_Data.AddPoint(Vector3(50, 150, 0), 2);
	l_Data.AddPoint(Vector3(250, 50, 0), 3);
	l_Data.AddPoint(Vector3(50, 250, 0), 4);
	l_Data.SetLOD(6);
	m_pTweenyCurveWithTime->SetData(tweeny::easing::enumerated::quadraticInOut,3,&l_Data,nullptr);
}

void cTweenTest::Update(float e_fElpaseTime)
{
	if (m_pTweenyCurveWithTime)
	{
		m_pTweenyCurveWithTime->Update(e_fElpaseTime);
	}
}

void cTweenTest::Render()
{
	if (m_pTweenyCurveWithTime)
	{
		m_pTweenyCurveWithTime->Render();
	}
	//cGameCamera::GetInstance()->Render();
	if (m_pTweenyTestObject)
	{
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
	if (m_pTweenyCurveWithTime)
	{
		m_pTweenyCurveWithTime->Render();
	}
}

void cTweenTest::KeyDown(char e_cKey)
{
	if (m_pTweenyTestObject)
	{
		m_pTweenyTestObject->KeyUp();
	}
	if (m_pTweenyCurveWithTime)
	{
		m_pTweenyCurveWithTime->Reset();
	}
}

void AddTestPhase(eTestPhase e_eTestPhase, cPhaseManager& e_PhaseManager, bool e_bSetCurrent)
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
		case eTP_BATTLE_ATTACK_MOVING_OBJECT_TESTING:
			if (!e_PhaseManager.GetObject(cBattleAttackMoveObjectTesting::TypeID))
			{
				cBattleAttackMoveObjectTesting* l_pTweenTest = new cBattleAttackMoveObjectTesting();
				e_PhaseManager.AddObject(l_pTweenTest);
			}
		break;
	}
	if (e_bSetCurrent)
	{
		SetWorkingTestPhase(e_eTestPhase, e_PhaseManager);
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
	case eTestPhase::eTP_BATTLE_ATTACK_MOVING_OBJECT_TESTING:
		e_PhaseManager.SetCurrentCurrentPhase(cBattleAttackMoveObjectTesting::TypeID);
		break;
	}
}