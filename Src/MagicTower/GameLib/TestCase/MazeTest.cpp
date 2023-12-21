#include "stdafx.h"
#include "MazeTest.h"
#include "../Level/MazeRender.h"
#include "../GameCamera/GameCamera.h"
#include "./GamePlayTesting.h"
#include "../Object/SpecialTimingCurveClickObject.h"

TYPDE_DEFINE_MARCO(cMazeTest);
TYPDE_DEFINE_MARCO(cTweenTest);
TYPDE_DEFINE_MARCO(cSpecialTimingCurveClickObjectTesting);


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
	cCurveWithTime*l_pData = new cCurveWithTime();
	l_pData->AddPoint(Vector3(50, 50, 0), 0);
	l_pData->AddPoint(Vector3(150, 50, 0), 1);
	l_pData->AddPoint(Vector3(50, 150, 0), 2);
	l_pData->AddPoint(Vector3(250, 50, 0), 3);
	l_pData->AddPoint(Vector3(50, 250, 0), 4);
	l_pData->SetLOD(6);
	m_pTweenyCurveWithTime->SetData(tweeny::easing::enumerated::quadraticInOut,3,l_pData,nullptr);
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
				cMazeTest*l_pData = new cMazeTest();
				e_PhaseManager.AddObject(l_pData);
			}
		break;
		case eTestPhase::eTP_TWEEN:
			if (!e_PhaseManager.GetObject(cTweenTest::TypeID))
			{
				cTweenTest* l_pData = new cTweenTest();
				e_PhaseManager.AddObject(l_pData);
			}
		break;
		case eTP_BATTLE_ATTACK_MOVING_OBJECT_TESTING:
			if (!e_PhaseManager.GetObject(cBattleAttackMoveObjectTesting::TypeID))
			{
				cBattleAttackMoveObjectTesting* l_pData = new cBattleAttackMoveObjectTesting();
				e_PhaseManager.AddObject(l_pData);
			}
		break;
		case eTP_SPECIAL_TIMING_CURVE_CLICK_OOBJECT_TESTING:
			if (!e_PhaseManager.GetObject(cSpecialTimingCurveClickObjectTesting::TypeID))
			{
				cSpecialTimingCurveClickObjectTesting* l_pData = new cSpecialTimingCurveClickObjectTesting();
				e_PhaseManager.AddObject(l_pData);
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
	case eTestPhase::eTP_SPECIAL_TIMING_CURVE_CLICK_OOBJECT_TESTING:
		e_PhaseManager.SetCurrentCurrentPhase(cSpecialTimingCurveClickObjectTesting::TypeID);
		break;
	}
}

cSpecialTimingCurveClickObjectTesting::cSpecialTimingCurveClickObjectTesting()
{
	this->SetName(cSpecialTimingCurveClickObjectTesting::TypeID);
	m_pLineImage = nullptr;
	m_pSpecialTimingCurveClickObject = new cSpecialTimingCurveClickObject();
	m_pSpecialTimingCurveClickObject->AssignTestingData();
	m_ClickBehaviorGroup.AddObject(m_pSpecialTimingCurveClickObject);
}

cSpecialTimingCurveClickObjectTesting::~cSpecialTimingCurveClickObjectTesting()
{
	SAFE_DELETE(m_pLineImage);
}

void cSpecialTimingCurveClickObjectTesting::Init()
{
	m_pLineImage = new cBaseImage("MagicTower/Image/V_Wall.png");
	m_pSpecialTimingCurveClickObject->Init();
}

void cSpecialTimingCurveClickObjectTesting::Update(float e_fElpaseTime)
{
	m_ClickBehaviorGroup.Update(e_fElpaseTime);
}

void cSpecialTimingCurveClickObjectTesting::Render()
{
	if (m_pSpecialTimingCurveClickObject)
	{
		auto l_Object = m_pSpecialTimingCurveClickObject->GetTweenyCurveWithTime();
		if (m_pLineImage)
		{
			cCurve* l_pCurve = l_Object->GetCurve();
			int l_iNumVertices = 0;
			const int l_ciTest = 9999;
			Vector3 l_OutPos[l_ciTest];
			Vector2 l_vOutUV[l_ciTest];
			Vector4 l_vOutColor[l_ciTest];
			m_pLineImage->GenerateCurveTriangulatorRenderDataForBatchRendering(l_pCurve, l_iNumVertices, l_OutPos, l_vOutUV, l_vOutColor, true);
			int a = 0;
		}
		m_pSpecialTimingCurveClickObject->Render();
	}
	cGameApp::ShowInfo();
}

void cSpecialTimingCurveClickObjectTesting::MouseMove(int e_iPosX, int e_iPosY)
{
	m_ClickBehaviorGroup.MouseMove(e_iPosX,e_iPosY);
}

void cSpecialTimingCurveClickObjectTesting::MouseDown(int e_iPosX, int e_iPosY)
{
	m_ClickBehaviorGroup.MouseDown(e_iPosX, e_iPosY);
}

void cSpecialTimingCurveClickObjectTesting::MouseUp(int e_iPosX, int e_iPosY)
{
	m_ClickBehaviorGroup.MouseUp(e_iPosX, e_iPosY);
}

void cSpecialTimingCurveClickObjectTesting::InternalMouseLeave(int e_iPosX, int e_iPosY)
{
}

void cSpecialTimingCurveClickObjectTesting::KeyDown(char e_cKey)
{
	if (m_pSpecialTimingCurveClickObject)
	{
		m_pSpecialTimingCurveClickObject->GetTweenyCurveWithTime()->Reset();
	}
	
}
