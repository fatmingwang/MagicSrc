#pragma once
//
class	cMonsterManager;
class	cPlayerManager;
class	cBulletManager;
class	cProbabilityFish;
class	cSceneChange;
class	cWinMoneyEffectManager;






class	cFishApp:public cGameApp
{
	cFrameBuffer* m_pFrameBuffer;
protected:
#ifdef WIN32
	virtual	void	OpenglInit(HWND e_Hwnd){}
#endif
public:
#if defined(WIN32)
	cFishApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920,1080),Vector2 e_vViewportSize = Vector2(1920,1080));
#elif defined(ANDROID)
	cFishApp(ANativeActivity* e_pActivity, JNIEnv* e_pThreadEnv, jobject* e_pAppThreadThis, Vector2 e_vGameResolution, Vector2 e_vViewportSize, NvEGLUtil* e_pNvEGLUtil);
#else
	cFishApp(Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT),Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH,IPHONE_RESOLUTION_HEIGHT));
#endif
	virtual ~cFishApp();
	void			Init();
	void			Update(float e_fElpaseTime);
	void			Render();
	void			GameDataDestroy();
	virtual	void	MouseDown(int e_iPosX,int e_iPosY);
	virtual	void	MouseMove(int e_iPosX,int e_iPosY);
	virtual	void	MouseUp(int e_iPosX,int e_iPosY);
	void			KeyDown(char e_char);
	void			KeyUp(char e_char);
	//if true game exit
	bool						m_bLeave;
	static  float				m_sfVersion;
	//=======================================
	static	cMonsterManager*		m_spMonsterManager;
	static	cPlayerManager*			m_spPlayerManager;
	static	cBulletManager*			m_pBulletManager;
	static  cProbabilityFish*		m_spProbabilityFish;
	static	cSceneChange*			m_spSceneChange;
	static  cWinMoneyEffectManager* m_spWinMoneyEffectManager;

	static	UINT64				m_sui64CurrentStep;
	static	float				m_sfMonsterUpdateSpeed;
	//while scene change player cannot shoot any bullet,and fish has leave scene quickly
	static	bool				m_sbSceneChange;
	static	bool				m_sbIsUsingMasterLeeProbability;
	static  float				m_MiniGameShakeTm;
	//<root FullScreen="0" Resolution="960,640" ViewPort="960,640" DeviceOrietation="0" />
	static void					ResoluctionParse2(char*e_strFileName);
};