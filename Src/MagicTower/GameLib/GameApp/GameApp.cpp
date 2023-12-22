#include "stdafx.h"
#include "GameApp.h"
#include "../Phase/SceneControl.h"
#include "../Object/MainCharacterBehavior.h"
#include "../UI/UIInfo.h"
#include "../FileNameDefine.h"
#include "../GameCamera/GameCamera.h"
#include "../TestCase/MazeTest.h"
//#include "../Level/MazeRender.h"

//cMazeRender* g_pCyucelenMazeGrid = nullptr;
//cTweenyTestObject* g_pTweenyTestObject = nullptr;


cSceneControl*					cMagicTowerApp::m_spSceneControl = 0;
cOrthogonalCamera*				cMagicTowerApp::m_sp2DCamera = 0;
cMainRoleData*					cMagicTowerApp::m_spMainRoleData = 0;
cMagicTowerApp*					g_pMagicTowerApp = 0;
bool							g_bGameLeave = false;


#ifdef WIN32
cMagicTowerApp::cMagicTowerApp(HWND e_Hwnd, Vector2 e_vGameResolution, Vector2 e_vViewportSize)
	:cGameApp(e_Hwnd, e_vGameResolution, e_vViewportSize)
#elif defined(ANDROID)
cMagicTowerApp::cMagicTowerApp(ANativeActivity* e_pActivity, JNIEnv* e_pThreadEnv, jobject* e_pAppThreadThis, Vector2 e_vGameResolution, Vector2 e_vViewportSize, NvEGLUtil* e_pNvEGLUtil) :
	cGameApp(e_pActivity, e_pThreadEnv, e_pAppThreadThis, e_vGameResolution, e_vViewportSize, e_pNvEGLUtil)
#else
cMagicTowerApp::cMagicTowerApp(Vector2 e_vGameResolution, Vector2 e_vViewportSize)
	: cGameApp(e_vGameResolution, e_vViewportSize)
#endif
{
	g_pMagicTowerApp = this;
	m_spSceneControl = 0;
	m_sp2DCamera = 0;
	m_pUIInfo = 0;
}

cMagicTowerApp::~cMagicTowerApp()
{
	g_bGameLeave = true;
	SAFE_DELETE(m_pUIInfo);
	SAFE_DELETE(m_spSceneControl);
	SAFE_DELETE(m_sp2DCamera);
	SAFE_DELETE(m_spMainRoleData);
	cTweenyManager::DestroyInstance();
}

void	cMagicTowerApp::Init()
{
	cGameApp::Init();
	bool l_bNotDoTest = false;
	if (l_bNotDoTest)
	{
		cNodeISAX	l_cNodeISAX;
		if (l_cNodeISAX.ParseDataIntoXMLNode(MAIN_ROLE_SETUP_DATA))
		{
			m_spMainRoleData = new cMainRoleData(l_cNodeISAX.GetRootElement());
		}
		m_sp2DCamera = new cOrthogonalCamera(Vector2(480, 320));
		m_spSceneControl = new cSceneControl();
		POINT	l_ViewSize = { 480,320 };
		m_sp2DCamera->SetResolution(Vector2(480, 320));
		//m_sp2DCamera->Update(l_ViewSize);
		if (m_spSceneControl)
			m_spSceneControl->Load();
		m_pUIInfo = new cUIInfo();
	}
	//AddTestPhase(eTestPhase::eTP_MAZE,this->m_PhaseManager,true);
	//AddTestPhase(eTestPhase::eTP_TWEEN, this->m_PhaseManager,true);
	//AddTestPhase(eTestPhase::eTP_BATTLE_ATTACK_MOVING_OBJECT_TESTING, this->m_PhaseManager,true);
	AddTestPhase(eTestPhase::eTP_SPECIAL_TIMING_CURVE_CLICK_OOBJECT_TESTING, this->m_PhaseManager, true);
}

void	cMagicTowerApp::Destory()
{

}

void	cMagicTowerApp::Update(float e_fElpaseTime)
{
	if (g_bGameLeave)
	{
		return;
	}
	cGameApp::Update(e_fElpaseTime);
	cTweenyManager::GetInstance()->Update(e_fElpaseTime);
	cGameCamera::GetInstance()->Update(e_fElpaseTime);
	if (m_PhaseManager.GetCurrentPhase() != -1)
	{
		m_PhaseManager.Update(e_fElpaseTime);
	}
	else
	{
		if (m_spSceneControl)
			m_spSceneControl->Update(e_fElpaseTime);
		if (m_pUIInfo)
			m_pUIInfo->Update(e_fElpaseTime);
	}
}

void	cMagicTowerApp::Render()
{
	if (g_bGameLeave)
	{
		return;
	}
	cGameApp::Render();
	if (m_PhaseManager.GetCurrentPhase() != -1)
	{
		m_PhaseManager.Render();
	}
	else
	{
		if (m_pUIInfo)
			m_pUIInfo->Render();
		m_sp2DCamera->Render();
		if (m_spSceneControl)
			m_spSceneControl->Render();
		if (m_spSceneControl)
			m_spSceneControl->DebugRender();
		cGameApp::RenderFont(0, 0, UT::ComposeMsgByFormat(L"Mouse:%d,%d", cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y).c_str());
		GLRender::glDisable2D();
	}
	RenderShowInfoOnScreen();
#ifdef WIN32
	SwapBuffers (this->m_spOpenGLRender->m_Hdc);
#endif
}

void	cMagicTowerApp::MouseDown(int e_iPosX,int e_iPosY)
{
	if( g_bGameLeave )
		return;
	cGameApp::MouseDown(e_iPosX, e_iPosY);
#ifdef WASM
	m_sMousePosition.y += EMSDK::EMSDK_GetCanvasPosY();
#endif
	if (m_spSceneControl)
	{
		this->m_spSceneControl->MouseDown(this->m_sMousePosition.x, this->m_sMousePosition.y);
	}
	m_PhaseManager.MouseDown(this->m_sMousePosition.x, this->m_sMousePosition.y);
}

void	cMagicTowerApp::MouseMove(int e_iPosX,int e_iPosY)
{
	if( g_bGameLeave )
		return;
	cGameApp::MouseMove(e_iPosX, e_iPosY);
#ifdef WASM
	m_sMousePosition.y += EMSDK::EMSDK_GetCanvasPosY();
#endif
	if (m_spSceneControl)
	{
		this->m_spSceneControl->MouseMove(this->m_sMousePosition.x, this->m_sMousePosition.y);
	}
	m_PhaseManager.MouseMove(this->m_sMousePosition.x, this->m_sMousePosition.y);
}

void	cMagicTowerApp::MouseUp(int e_iPosX,int e_iPosY)
{
	if( g_bGameLeave )
		return;
	cGameApp::MouseUp(e_iPosX, e_iPosY);
#ifdef WASM
	m_sMousePosition.y += EMSDK::EMSDK_GetCanvasPosY();
	static bool l_sbDoOnce = true;
	if (l_sbDoOnce)
	{
		l_sbDoOnce = false;
		EMSDK::EMSDK_JSDoFullscreen();
	}
#endif
	if (m_spSceneControl)
	{
		this->m_spSceneControl->MouseUp(this->m_sMousePosition.x, this->m_sMousePosition.y);
	}
	m_PhaseManager.MouseUp(this->m_sMousePosition.x, this->m_sMousePosition.y);
}

void	cMagicTowerApp::KeyDown(char e_char)
{
	if( g_bGameLeave )
		return;
	m_PhaseManager.KeyDown(e_char);
	cGameCamera::GetInstance()->KeyUp(e_char);
	//cGameApp::KeyDown(e_char);
	if (m_spSceneControl)
	{
		m_spSceneControl->KeyDown(e_char);
	}
	m_PhaseManager.KeyDown(e_char);
}

void	cMagicTowerApp::ChangeWalkingView()
{
	if( g_bGameLeave )
		return;
	if( m_spSceneControl )
	{
		cWalkingInMapPhase*l_pWalkingInMapPhase = (cWalkingInMapPhase*)cMagicTowerApp::m_spSceneControl->GetObject(WALKING_IN_MAP_PHASE);
		l_pWalkingInMapPhase->SetWalkingModelToFix(!l_pWalkingInMapPhase->IsWalkingModeIsFix());
	}
}