#include "stdafx.h"
#include "Resource.h"
#include "Windowsx.h"


#include "../GameLib/GameApp/GameApp.h"
//
#define MAX_LOADSTRING 100
//
// Global Variables:
HINSTANCE			hInst;								// current instance
TCHAR				szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR				szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND				g_hWnd;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
HHOOK MouseHook;
bool				g_bLeave = false;
cGameApp			*g_pGameApp = 0;

#include <direct.h>



void	TweenyTest()
{
	//tween.tcc line 113.
	//https://easings.net/zh-tw
	//https://github.com/mobius3/tweeny-demos
	auto tween1 = tweeny::from(0.0, 1.0f).to(1.0f, 0.0f).via(tweeny::easing::enumerated::stepped, tweeny::easing::linear);
	//auto tween1 = tweeny::from(0.0, 1.0f).to(1.0f, 0.0f).via("stepped", "linear");
	auto l_Array2 = tween1.step(1);
	//tween1.step()
	auto helloworld = tweeny::from('h', 'e', 'l', 'l', 'o').to('w', 'o', 'r', 'l', 'd').during(50);
	for (int i = 0; i < 50; i++)
	{
		auto l_str = helloworld.step(1);
		std::string l_strResult = UT::ComposeMsgByFormat("%d: ", i);
		for (char c : l_str)
		{
			l_strResult += c;
		}
		FMLOG(l_strResult.c_str());
	}// 
}



int APIENTRY _tWinMain( HINSTANCE hInstance,
                        HINSTANCE hPrevInstance,
                        LPTSTR    lpCmdLine,
                        int       nCmdShow )
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	FMLog::Init();
	//TweenyTest();
 	// TODO: Place code here.
	MSG msg;
	//HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	swprintf( szTitle, 100, L"FSC");
	LoadString(hInstance, IDC_TEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	} // end if
	g_pGameApp = new cMagicTowerApp( g_hWnd, Vector2(1280,720), Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height()));
	g_pGameApp->Init();
	cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), (int)cGameApp::m_spOpenGLRender->m_vViewPortSize.Height(), (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	SetTimer( g_hWnd, 0, 0, NULL ) ;

    //MouseHook = SetWindowsHookEx(WH_MOUSE_LL,MouseHookProc,hInstance,0);
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)&&!g_bLeave)
	//while(1)
	{
		//g_pGameApp->Run();
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	//UnhookWindowsHookEx(MouseHook);
	SAFE_DELETE(g_pGameApp);
	_CrtDumpMemoryLeaks();
	NamedTypedObject::DumpUnReleaseInfo();
	return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//







ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= 0;//MAKEINTRESOURCE(IDC_TEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//







BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
	bool	l_bFullScreen = false;
	cNodeISAX	l_NodeISAX;
	cGameApp::m_spOpenGLRender = new cOpenGLRender(Vector2(1280, 720));
	auto &l_vResolution = cGameApp::m_spOpenGLRender->m_vViewPortSize;

	//cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0;
	//cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0;
	//cGameApp::m_spOpenGLRender->m_vViewPortSize.z = 1024.f;
	//cGameApp::m_spOpenGLRender->m_vViewPortSize.w = 768.f;

	DWORD	l_dwFlag = WS_OVERLAPPEDWINDOW;
	//auto l_vResolution = cGameApp::m_spOpenGLRender->m_vViewPortSize;
	//auto l_Size = l_vResolution.Size();
	//POINT l_Size = {720,1280};
	POINT l_Size = { 1280,720 };
	if(cGameApp::m_sbFullScreen)
		l_dwFlag = WS_VISIBLE | WS_POPUP |	WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	g_hWnd = CreateWindow(szWindowClass, szTitle, l_dwFlag, 0, 0, (int)l_Size.x, (int)l_Size.y, NULL, NULL, hInstance, NULL);

	if (!g_hWnd)
	{
	  return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
POINT g_MousePosition;
bool	g_bLeftMouseDown = false;
bool	g_bRightMouseDown = false;
short	g_cMouseWhellDelta = 0;
bool	g_bTimerDone = false;







LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	g_cMouseWhellDelta = 0;
	float   l_fScaleX = cGameApp::m_spOpenGLRender->m_vGameResolution.x/ cGameApp::m_spOpenGLRender->m_vViewPortSize.x;
	float   l_fScaleY = cGameApp::m_spOpenGLRender->m_vGameResolution.y/ cGameApp::m_spOpenGLRender->m_vViewPortSize.y;
	auto l_ViewRect = &cGameApp::m_spOpenGLRender->m_vViewPortSize;
	switch (message)
	{
	case  WM_SIZE:
		cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution((int)LOWORD(lParam), (int)HIWORD(lParam), (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		//cGameApp::m_spOpenGLRender->m_vViewPortSize.w = 
		break;
	case WM_TIMER:
		if( !g_bLeave && g_pGameApp && g_bTimerDone == false )
		{
			g_bTimerDone = true;
			g_pGameApp->Run();
			g_bTimerDone = false;
		}
		//else
		//	SAFE_DELETE(g_pGameApp);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	//case WM_KEYFIRST:
	//	if( g_pGameApp )
	//	{
	//		g_pGameApp->KeyDown(LOWORD(wParam));
	//	}
	//	break;

	case WM_KEYDOWN:
		if( g_pGameApp )
		{
			g_pGameApp->KeyDown((char)LOWORD(wParam));
		}
		switch(LOWORD(wParam))
		{
			case VK_RETURN:
			{

			}
			break;
			case VK_ESCAPE:
				if(g_pGameApp->m_sbDebugFunctionWorking)
				{
					g_bLeave = true;
				}
				break;
			break;
		}
		break;
	case WM_KEYUP:
		if( g_pGameApp )
		{
			g_pGameApp->KeyUp((char)LOWORD(wParam));
		}
		break;
	//case WM_MOUSEHWHEEL:
	case WM_MOUSEWHEEL:
		g_cMouseWhellDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		break;
	case WM_LBUTTONDOWN:
		g_bLeftMouseDown = true;
		g_bRightMouseDown = false;
		if( g_pGameApp )
			g_pGameApp->MouseDown(g_MousePosition.x,g_MousePosition.y);
			return 0;
	case WM_LBUTTONUP:
		g_bLeftMouseDown = false;
		if( g_pGameApp )
			g_pGameApp->MouseUp(g_MousePosition.x,g_MousePosition.y);
			return 0;
	case WM_RBUTTONDOWN:
		g_bRightMouseDown = true;
		g_bLeftMouseDown = false;
			return 0;
	case WM_RBUTTONUP:
		g_bRightMouseDown = false;
			return 0;
	case WM_MOUSEMOVE:
		g_MousePosition.x = (int)(GET_X_LPARAM(lParam));
		g_MousePosition.y = (int)(GET_Y_LPARAM(lParam));
		g_pGameApp->m_sMousePosition = g_MousePosition;
		if( g_pGameApp )
			g_pGameApp->MouseMove(g_MousePosition.x,g_MousePosition.y);

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}








