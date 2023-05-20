#include "stdafx.h"
#include "GameApp.h"
#include "SceneChange.h"
#include "../Monster/MonsterManager.h"
#include "../PlayerBehavior/PlayerData.h"
#include "../GameBullet/BulletManager.h"
#include "../ProbabilityFish/FishProbability.h"
#include "../UI/GameUI.h"
#include "../GameCamera/GameCamera.h"
#ifdef WIN32
#include "winioctl.h"
#include <intrin.h>
#endif
//
UINT					g_uiFrame = 0;
//
float					cFishApp::m_sfVersion = 1.0f;
cMonsterManager*		cFishApp::m_spMonsterManager = 0;
cPlayerManager*			cFishApp::m_spPlayerManager = 0;
cBulletManager*			cFishApp::m_pBulletManager = 0;
cProbabilityFish*		cFishApp::m_spProbabilityFish = 0;
cSceneChange*			cFishApp::m_spSceneChange = 0;
bool					cFishApp::m_sbSceneChange = false;
float					cFishApp::m_sfMonsterUpdateSpeed = 1.f;
UINT64					cFishApp::m_sui64CurrentStep = 0;
float					cFishApp::m_MiniGameShakeTm = 0.f;
bool					cFishApp::m_sbIsUsingMasterLeeProbability = false;

bool*					g_pbIsUsingMasterLeeProbability = &cFishApp::m_sbIsUsingMasterLeeProbability;
extern bool	g_bLeave;
bool		g_bInitOk = false;
extern bool		g_bProtected;

int	g_iLoadingStep = 1;

void	LoadingProgressInfo()
{	
	cGameApp::m_sTimeAndFPS.Update();
	//if( cGameApp::m_sTimeAndFPS.fElpaseTime >0.032f )
	{
		cGameApp::m_spOpenGLRender->Render();
		const wchar_t*l_strLoadingInfo = L"Loading...";
		int	l_iLength = (int)wcslen(l_strLoadingInfo);
		wchar_t	l_str[MAX_PATH];
		memcpy(l_str,l_strLoadingInfo,sizeof(wchar_t)*g_iLoadingStep);
		l_str[g_iLoadingStep] = L'\0';
		//UseShaderProgram();
		auto l_vVeiewRect = cGameApp::m_spOpenGLRender->m_vViewPortSize;
		auto l_vGameResolution = cGameApp::m_spOpenGLRender->m_vGameResolution;
		glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		cGameApp::RenderFont(cGameApp::m_spOpenGLRender->m_vGameResolution / 2, L"loading");
#ifdef WIN32
		SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
		++g_iLoadingStep;
		if( g_iLoadingStep >= l_iLength )
			g_iLoadingStep = 1;
	}
}

#ifdef WIN32
cFishApp::cFishApp(HWND e_Hwnd,Vector2 e_vGameResolution,Vector2 e_vViewportSize)
:cGameApp(e_Hwnd,e_vGameResolution,e_vViewportSize)
#elif defined(ANDROID)
cFishApp::cFishApp(ANativeActivity* e_pActivity, JNIEnv* e_pThreadEnv, jobject* e_pAppThreadThis, Vector2 e_vGameResolution, Vector2 e_vViewportSize, NvEGLUtil* e_pNvEGLUtil):
cGameApp(e_pActivity,e_pThreadEnv,e_pAppThreadThis,e_vGameResolution,e_vViewportSize,e_pNvEGLUtil)
#else
cFishApp::cFishApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize)
:cGameApp(e_vGameResolution,e_vViewportSize)
#endif
{
	m_pFrameBuffer = nullptr;
	cGameApp::m_spOpenGLRender->m_vBGColor = Vector4(0.1f, 0.1f, 0.1f, 1.f);
	Vector2* l_pGameRes = &cGameApp::m_spOpenGLRender->m_vGameResolution;
	cGameApp::m_spOpenGLRender->m_vGameResolution.x = 720;
	cGameApp::m_spOpenGLRender->m_vGameResolution.y = 1280;
	g_sbCollisionRender = true;
	cGameApp::Init();
	FMLog::Log("constructor Start",false);
	if( 1 )
	{
		//cCurveWithTime	l_cCurveWithTime;
		//std::vector<cCurveWithTime>	ll;
		//ll.push_back( l_cCurveWithTime );
		//cFMAnimationRule	l_cFMAnimationRule;
		//std::vector<cFMAnimationRule>	aa;
		//aa.push_back(l_cFMAnimationRule);
	}
	//m_sbSpeedControl = true;
	m_bLeave = false;

	//if(!cGameApp::m_spSoundParser->Parse("Fish/Sound/Sound.xml"))
	//{
	//	UT::ErrorMsg(L"Fish/Sound.xml",L"parse sound error");
	//}
	FMLog::Log("constructor End",false);
	//cGameApp::m_spSoundParser->GetObject(L"3")->SetVolume(0.1f);
}


cFishApp::~cFishApp()
{
	g_bInitOk = false;
	SAFE_DELETE(m_pFrameBuffer);
	GameDataDestroy();
	Destroy();
}

void	cFishApp::Init()
{
	this->m_spSoundParser->SetVolume(0);
	FMLOG("cFishApp::Init 1");
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	g_bInitOk = false;
	LoadingProgressInfo();
	m_sfMonsterUpdateSpeed = 1.f;
	m_sbSceneChange = false;
	//cGameApp::SoundPlay(L"bg",false);
	//cGameApp::SoundPlay(L"bg02",false);
	//cGameApp::SoundPlay(L"gold",false);
#ifdef DEBUG
	//cGameApp::SoundPlay(L"logo",false);
#else
	//cGameApp::SoundPlay(L"logo",true);
#endif
	//cGameApp::SoundPlay(L"6",false);
	FMLOG("cFishApp::Init 2");
	if( UT::IsFileExists("Fish/Image/Loading.png") )
	{
		//UseShaderProgram();
		cGameApp::m_spOpenGLRender->Render();
		//glViewport(0,0,(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(),(int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height());
		//glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
		//glClearColor( 0,0.5,0.5,1 );
		//glClearDepth(1.0f);	
		//glEnable(GL_TEXTURE_2D);
		//glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		cBaseImage*l_pLoadingImage = new cBaseImage("Fish/Image/Loading.png");
		l_pLoadingImage->Render();
#ifdef WIN32
		SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
		SAFE_DELETE(l_pLoadingImage);
	} 
	//
	FMLOG("cFishApp::Init 3");
	GameDataDestroy();
	FMLOG("cFishApp::Init 4");
	cGameUI::GetInstance()->Init();
	FMLOG("cFishApp::Init 5");
	FMLOG("cFishApp::Init 6");
#define	GAME_RESOLUTION_WIDTH	1280.f
#define	GAME_RESOLUTION_HEIGHT	720.f
	cGameApp::m_spOpenGLRender->m_vGameScale.x = cGameApp::m_spOpenGLRender->m_vGameResolution.x/GAME_RESOLUTION_WIDTH;
	cGameApp::m_spOpenGLRender->m_vGameScale.y = cGameApp::m_spOpenGLRender->m_vGameResolution.y/GAME_RESOLUTION_HEIGHT;
	m_spSceneChange = new cSceneChange();
	FMLOG("cFishApp::Init 7");
	m_pBulletManager = new cBulletManager();
	FMLOG("cFishApp::Init 8");
	m_spMonsterManager = new cMonsterManager();
	FMLOG("cFishApp::Init 10");
	m_spPlayerManager = new cPlayerManager();
	FMLOG("cFishApp::Init 11");
	m_spProbabilityFish = new cProbabilityFish();
	FMLOG("cFishApp::Init 12");
	FMLOG("cFishApp::Init 14");
	FMLOG("cFishApp::Init 15");
	//PrintMemoryInfo();
	//PrintMemoryInfo();
	if( m_spMonsterManager )
		m_spMonsterManager->Init();
	//PrintMemoryInfo();
	if( m_spPlayerManager )
		m_spPlayerManager->Init();
	//PrintMemoryInfo();
	if( m_pBulletManager )
		m_pBulletManager->Init();
	//
	if (m_spProbabilityFish)
		m_spProbabilityFish->Init();
	//PrintMemoryInfo();
	if( m_spSceneChange )
		m_spSceneChange->Init();
	//PrintMemoryInfo();
	this->m_sTimeAndFPS.Update();
	//g_sbCollisionRender = false;
	g_bInitOk = true;
	this->m_spSoundParser->SetVolume(0);

	//m_pFrameBuffer = new cFrameBuffer(720, 1280);
	//m_pFrameBuffer = new cFrameBuffer(720/4, 1280/4);
	FMLOG("cFishApp::Init all done");
}

void	cFishApp::GameDataDestroy()
{
	cGameUI::DestroyInstance();
	cGameCamera::DestroyInstance();
	SAFE_DELETE(m_spProbabilityFish);
	SAFE_DELETE(m_spMonsterManager);
	SAFE_DELETE(m_spPlayerManager);
	SAFE_DELETE(m_pBulletManager);
	SAFE_DELETE(m_spSceneChange);
}

//void	cFishApp::Destroy()
//{
//#ifdef WIN32
//	Delete2DShader(L"NoTexture");
//	Delete2DShader();
//#endif
//}

enum	eDebugInfo
{
	eDI_GAMEAPP = 0,
	eDI_SCENE_CHANGE,
	eDI_MINI_GAME,
	eDI_MONSTER,
	eDI_PLAYER,
	eDI_BULLET,//5
	eDI_PROBABILITY,
	eDI_WINMONEYEFFECT,
	eDI_MAX
};

const int	g_iAllDebugInfo = eDI_MAX;
float	g_fUpdateUsingTime[g_iAllDebugInfo];
float	g_fRenderTime[g_iAllDebugInfo];

const wchar_t*g_strDebugInfo[g_iAllDebugInfo] = 
{
							L"GameApp",
							L"SceneChange",
							L"MiniGameManager",
							L"MonsterManager",
							L"PlayerManager",
							L"BulletManager",
							L"FishProbability",
							L"WinMoneyEffectManager"};
UT::sTimeAndFPS	g_TimeStamp;

void	TimsStamp(eDebugInfo e_eDebugInfo,bool e_bUpdate,bool e_bTimeAdded = false)
{
	g_TimeStamp.Update();
	if( e_bUpdate )
	{
		cFishApp::m_sui64CurrentStep = e_eDebugInfo;
		if( e_bTimeAdded )
			g_fUpdateUsingTime[e_eDebugInfo] += g_TimeStamp.fElpaseTime;
		else
			g_fUpdateUsingTime[e_eDebugInfo] = g_TimeStamp.fElpaseTime;
	}
	else
	{
		cFishApp::m_sui64CurrentStep = e_eDebugInfo+100;
		if( e_bTimeAdded )
			g_fRenderTime[e_eDebugInfo] += g_TimeStamp.fElpaseTime;
		else
			g_fRenderTime[e_eDebugInfo] = g_TimeStamp.fElpaseTime;
	}
}

void	cFishApp::ResoluctionParse2(char*e_strFileName)
{
	cNodeISAX	l_NodeISAX;
	bool	l_bFullScreen = false;
	bool	l_b = l_NodeISAX.ParseDataIntoXMLNode(e_strFileName);
	if(l_b)
	{
		TiXmlElement*l_pTiXmlElement = l_NodeISAX.GetRootElement();
		const wchar_t*l_strFullscreen = l_pTiXmlElement->Attribute(L"FullScreen");
		if( l_strFullscreen )
			m_sbFullScreen = _wtoi(l_strFullscreen)?true:false;
		const wchar_t*l_strResolution = l_pTiXmlElement->Attribute(L"Resolution");
		const wchar_t*l_strViewPort = l_pTiXmlElement->Attribute(L"ViewPort");
		const wchar_t*l_strDeviceOrietation = l_pTiXmlElement->Attribute(L"DeviceOrietation");
		const wchar_t*l_strHideMouseCursor = l_pTiXmlElement->Attribute(L"HideMouseCursor");
		const wchar_t*l_strTexturePowerOfTwo = l_pTiXmlElement->Attribute(L"TexturePowerOfTwo");
		const wchar_t*l_strDebugFunctionWorking = l_pTiXmlElement->Attribute(L"DebugFunction");

		if( l_strDebugFunctionWorking )
		{
			m_sbDebugFunctionWorking = _wtoi(l_strDebugFunctionWorking)?true:false;
		}		

		if( l_strResolution )
		{
			POINT	l_Resolution = GetPoint(l_strResolution);
			if( !l_strViewPort )
			{
				//cGameApp::m_spOpenGLRender->m_vGameResolution.x = (float)l_Resolution.x;
				//cGameApp::m_spOpenGLRender->m_vGameResolution.y = (float)l_Resolution.y;
			}
		}
		if( l_strViewPort )
		{
			POINT	l_Resolution = GetPoint(l_strViewPort);
			cGameApp::m_spOpenGLRender->m_vViewPortSize.z = 0;
			cGameApp::m_spOpenGLRender->m_vViewPortSize.w = 0;
			cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)l_Resolution.x;
			cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)l_Resolution.y;
		}
		if( l_strDeviceOrietation )
		{
			int	l_iIndex = _wtoi(l_strDeviceOrietation);
			if( l_iIndex <= eDD_LANDSCAPE_RIGHT )
				cGameApp::m_spOpenGLRender->m_eDeviceDirection = (eDeviceDirection)l_iIndex;
		}
		if( l_strHideMouseCursor )
		{
#ifdef WIN32
			ShowCursor(_wtoi(l_strHideMouseCursor)==0?false:true);
#endif
		}
		if( l_strTexturePowerOfTwo )
		{
			g_bSupportNonPowerOfTwoTexture = _wtoi(l_strTexturePowerOfTwo)?true:false;
		}
	}
}

void	cFishApp::Update(float e_fElpaseTime)
{
	//e_fElpaseTime = 0.016f;
//#ifndef DEBUG//avoid frame skip.
//	if( e_fElpaseTime>1/30.f )
//		e_fElpaseTime = 1/30.f;
//#endif
	g_TimeStamp.Update();
	if ( m_MiniGameShakeTm > 0.f )
	{
		m_MiniGameShakeTm -= e_fElpaseTime;

		if ( m_MiniGameShakeTm < 0.f )
			m_MiniGameShakeTm = 0.f;
	}

	cGameApp::Update(e_fElpaseTime);
	TimsStamp(eDI_GAMEAPP,true);
	if( m_spSceneChange )
		m_spSceneChange->Update(e_fElpaseTime);
	TimsStamp(eDI_SCENE_CHANGE,true);
	cGameUI::GetInstance()->Update(e_fElpaseTime);
	cGameCamera::GetInstance()->Update(e_fElpaseTime);
	TimsStamp(eDI_MINI_GAME,true);
	if( m_spMonsterManager )
		m_spMonsterManager->Update(e_fElpaseTime*m_sfMonsterUpdateSpeed);
	TimsStamp(eDI_MONSTER,true);
	if( m_spPlayerManager )
		m_spPlayerManager->Update(e_fElpaseTime);
	TimsStamp(eDI_PLAYER,true);
	TimsStamp( eDI_WINMONEYEFFECT, true );
	if( m_pBulletManager )
	{
		m_pBulletManager->Update(e_fElpaseTime);
		if( m_spMonsterManager )
		{
			m_pBulletManager->Collide(m_spMonsterManager);
		}
	}
	TimsStamp(eDI_BULLET,true);
	//must update at last.
	TimsStamp(eDI_PROBABILITY,true);
	if (m_spProbabilityFish)
	{
		m_spProbabilityFish->Update(e_fElpaseTime);
	}
	++g_uiFrame;
}

void	cFishApp::Render()
{
	cGameApp::Render();
	if (m_pFrameBuffer)
	{
		m_pFrameBuffer->StartDraw();
	}
	//glAlphaFunc(GL_GREATER,0.001f);	
	g_TimeStamp.Update();
	cGameCamera::GetInstance()->Render();
	TimsStamp(eDI_GAMEAPP,false);
	if( m_spSceneChange )
		m_spSceneChange->Render();
	TimsStamp(eDI_SCENE_CHANGE,false);
	TimsStamp(eDI_MINI_GAME,false);
	if( m_spMonsterManager )
		m_spMonsterManager->Render();
	TimsStamp(eDI_MONSTER,false);
	if( m_pBulletManager )
		m_pBulletManager->Render();
	if( m_spMonsterManager )
		m_spMonsterManager->LastRender();
	TimsStamp(eDI_BULLET,false);
	TimsStamp(eDI_PROBABILITY,false);
	if(m_spSceneChange)
		m_spSceneChange->LastRender();
	if( m_spPlayerManager )
		m_spPlayerManager->Render();
	TimsStamp(eDI_PLAYER,false);
	TimsStamp(eDI_MINI_GAME,false,true);
	TimsStamp( eDI_WINMONEYEFFECT, false );
	TimsStamp(eDI_SCENE_CHANGE,false,true);
	//glEnable2D(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);
#ifdef DEBUG
	if (m_sbDebugFunctionWorking)
	{
		if( m_spMonsterManager  )
			m_spMonsterManager->DebugRender();
		if( m_pBulletManager  )
			m_pBulletManager->DebugRender();

		//if(m_spSceneChange)
		//	m_spSceneChange->DebugRender();
		//std::wstring	l_str = UT::ComposeMsgByFormat( L"Frame:%d", g_uiFrame );
		//cGameApp::m_spGlyphFontRender->RenderFont( 0, 650, l_str.c_str() );
	}
#endif
	cGameUI::GetInstance()->Render();
	cGameApp::m_spGlyphFontRender->RenderFont(0, 5, UT::CharToWchar(cGameApp::m_sTimeAndFPS.GetFPS()).c_str());
#ifdef WIN32
	//for (int i = 0; i < g_iAllDebugInfo; ++i)
	//{
	//	std::wstring	l_str = UT::ComposeMsgByFormat(L"%ls:%.4f,%.4f", g_strDebugInfo[i], g_fUpdateUsingTime[i], g_fRenderTime[i]);
	//	cGameApp::m_spGlyphFontRender->RenderFont(10, 100+60 * i, l_str.c_str());
	//}
#endif
	if (m_pFrameBuffer)
	{
		m_pFrameBuffer->EndDraw();
		//cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height(), (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		POINT l_Pos = { 0,0 };
		POINT l_Size = { 720,1280 };
		m_pFrameBuffer->DrawBuffer(l_Pos, l_Size);
	}
#ifdef DEBUG
	cGameCamera::GetInstance()->Render();
	cGameCamera::GetInstance()->DebugRender();
#endif
#ifdef WIN32
	SwapBuffers(cGameApp::m_spOpenGLRender->m_Hdc);
#endif
}

void	cFishApp::MouseDown(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseDown(e_iPosX,e_iPosY);
#ifdef WASM
	m_sMousePosition.y += EMSDK::EMSDK_GetCanvasPosY();
#endif
	cGameUI::GetInstance()->MouseDown(m_sMousePosition.x, m_sMousePosition.y);
}

void	cFishApp::MouseMove(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseMove(e_iPosX,e_iPosY);
#ifdef WASM
	m_sMousePosition.y += EMSDK::EMSDK_GetCanvasPosY();
#endif
	//cGameUI::GetInstance()->MouseMove(m_sMousePosition.x, m_sMousePosition.y);
}

void	cFishApp::MouseUp(int e_iPosX,int e_iPosY)
{
    cGameApp::MouseUp(e_iPosX,e_iPosY);
#ifdef WASM
	m_sMousePosition.y += EMSDK::EMSDK_GetCanvasPosY();
	static bool l_sbDoOnce = true;
	if (l_sbDoOnce)
	{
		l_sbDoOnce = false;
		EMSDK::EMSDK_JSDoFullscreen();
	}
#endif
	cGameUI::GetInstance()->MouseUp(m_sMousePosition.x, m_sMousePosition.y);
}

void	cFishApp::KeyDown(char e_char)
{
	 cGameApp::KeyDown(e_char);
}
void	cFishApp::KeyUp(char e_char)
{
	cGameApp::KeyUp(e_char);
	cGameCamera::GetInstance()->KeyUp(e_char);
	if( e_char == 8 )
	{
		g_sbCollisionRender = !g_sbCollisionRender;
	}

	if( cGameApp::m_sbDebugFunctionWorking )
	{

		if( e_char == 8 )
		{
			g_sbCollisionRender = !g_sbCollisionRender;
		}
	}
}