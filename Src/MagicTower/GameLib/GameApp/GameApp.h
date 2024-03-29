#pragma once

	class	cSceneControl;
	class	cMainRoleData;
	class	cUIInfo;
	class	cMagicTowerApp:public cGameApp
	{
		cUIInfo*	m_pUIInfo;
		void		Update(float e_fElpaseTime);
		void		Render();
	public:
#if defined(ANDROID)
		cMagicTowerApp(ANativeActivity* e_pActivity,JNIEnv*e_pThreadEnv,jobject* e_pAppThreadThis,Vector2 e_vGameResolution,Vector2 e_vViewportSize );
#elif defined(WIN32)
		cMagicTowerApp(HWND e_Hwnd,Vector2 e_vGameResolution = Vector2(1920.f,1080.f),Vector2 e_vViewportSize = Vector2(1920.f,1080.f));
		cMagicTowerApp(Vector2 e_vGameResolution,Vector2 e_vViewportSize );
#else
		cMagicTowerApp(Vector2 e_vGameResolution = Vector2(IPHONE_RESOLUTION_WIDTH, IPHONE_RESOLUTION_HEIGHT), Vector2 e_vViewportSize = Vector2(IPHONE_RESOLUTION_WIDTH, IPHONE_RESOLUTION_HEIGHT));
#endif
		~cMagicTowerApp();
		void		Init();
		void		Destory();
		void		MouseDown(int e_iPosX,int e_iPosY);
		void		MouseMove(int e_iPosX,int e_iPosY);
		void		MouseUp(int e_iPosX,int e_iPosY);
		void		KeyDown(char e_char);
		void		ChangeWalkingView();
		cUIInfo*	GetUIINfo(){return m_pUIInfo;}
		static		cOrthogonalCamera*		m_sp2DCamera;
		static		cSceneControl*			m_spSceneControl;
		static		cMainRoleData*			m_spMainRoleData;
		//
		cPhaseManager m_PhaseManager;
	};
	extern	cMagicTowerApp*g_pMagicTowerApp;
