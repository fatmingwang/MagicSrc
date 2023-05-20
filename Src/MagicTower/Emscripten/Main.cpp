﻿#include <emscripten.h>
#include <SDL/SDL.h>


#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <assert.h>

#include <emscripten/emscripten.h>
#include <emscripten/websocket.h>

#include "../../../../FM79979/Src/FM79979Engine/Core/AllCoreInclude.h"
#include "../FTCGameLib/GameApp/GameApp.h"

cGameApp* g_pGameApp = 0;
cPreLoadFromInternet* g_pPreLoadFromInternet = nullptr;

//https://www.khronos.org/webgl/wiki/HandlingHighDPI
EM_JS
(
	void, ResizeCanvans, (),
	{
		//Module.requestFullscreen(false,true);
		var canvas = document.getElementById('canvas');
		var size = 200;
		canvas.style.width = 1280 + "px";
		canvas.style.height = 720 + "px";
	}
);

//https://stackoverflow.com/questions/51343425/not-able-to-bind-function-in-emscripten
//https://segmentfault.com/a/1190000011229465
//#include <emscripten/bind.h>
//================================12345
void handle_key_up(SDL_keysym* keysym)
{
	FMLOG("key up%d", (char)keysym->sym);
	g_pGameApp->KeyUp((char)keysym->sym);
	switch (keysym->sym)
	{
	case SDLK_ESCAPE:
		break;
	case SDLK_SPACE:

		break;
	default:
		break;
	}
}

void handle_key_down(SDL_keysym* keysym)
{
	FMLOG("key down%d", (char)keysym->sym);
	g_pGameApp->KeyDown((char)keysym->sym);
	switch (keysym->sym)
	{
	case SDLK_ESCAPE:
		break;
	case SDLK_SPACE:

		break;
	default:
		break;
	}
}

void process_events(void)
{
	static bool l_bDoOnce = true;
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (g_pGameApp)
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				handle_key_down(&event.key.keysym);
				break;
			case SDL_KEYUP:
				handle_key_up(&event.key.keysym);
				break;
			case SDL_QUIT:
				break;
			case SDL_MOUSEMOTION:
				g_pGameApp->MouseMove(event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONDOWN:
				g_pGameApp->MouseDown(event.motion.x, event.motion.y);
				break;
			case SDL_MOUSEBUTTONUP:
				if (l_bDoOnce)
				{
					l_bDoOnce = false;
					//ResizeCanvans();
				}
				g_pGameApp->MouseUp(event.motion.x, event.motion.y);
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_RESIZED ||
					event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED || event.window.event == SDL_WINDOWEVENT_MAXIMIZED ||
					event.window.event == SDL_WINDOWEVENT_RESTORED)
				{
					//ResizeCanvans();
					if (cGameApp::m_spOpenGLRender)
					{
						printf("size change:%d,%d\n", event.window.data1, event.window.data2);
						cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution(event.window.data2, event.window.data1,
							(int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
					}
				}
				break;
			}
		}
	}
}

void Loop()
{
	if (!g_pGameApp)
	{
		return;
	}
	g_pGameApp->Run();
	if (g_pPreLoadFromInternet)
	{
		g_pPreLoadFromInternet->Run();
		if (g_pPreLoadFromInternet->GetProgress() >= 1.f)
		{
			SAFE_DELETE(g_pPreLoadFromInternet);
			cGameApp::OutputDebugInfoString("finish pre-download files");
			g_pGameApp->Init();
			g_pGameApp->m_spOpenGLRender->m_vBGColor = Vector4::Red;
		}
	}
	else
	{
		process_events();
	}
	SDL_GL_SwapBuffers();
}

int sockfd = -1;
void finish(int result) {
	close(sockfd);
#ifdef __EMSCRIPTEN__
	//REPORT_RESULT(result);
	emscripten_force_exit(result);
#else
	exit(result);
#endif
}


int main()
{
	//c+/c++
	//--profiling -pthread -s PROXY_POSIX_SOCKETS=1 -s USE_PTHREADS=1 -s PROXY_TO_PTHREAD=1
	//no websocket --profiling -pthread 
	//linker
	//-s ERROR_ON_UNDEFINED_SYMBOLS=1 -s FETCH=1 -s FULL_ES3=1 -s ASSERTIONS=0 --bind -pthread  -s WEBSOCKET_SUBPROTOCOL=null -lwebsocket.js  -s USE_PTHREADS=1 -s SOCKET_DEBUG=1 -s PROXY_POSIX_SOCKETS=1 -s PROXY_TO_PTHREAD=1 -s WEBSOCKET_URL=192.168.31.242 
	// no tcp websocket.
	//-s ERROR_ON_UNDEFINED_SYMBOLS=1 -s FETCH=1 -s FULL_ES3=1 -s ASSERTIONS=0 --bind -pthread -lwebsocket.js  -s USE_PTHREADS=1 -s SOCKET_DEBUG=1 
	//if (1)
	//{
	//	printf("try to create websocket\n");
	//	//dire server
	//	//ws://sitapi.diresoft.net/api/agent
	//	//wss://funplayapi.pypc.net/api/agent
	//	//my
	//	//wss://localhost:9992
	//	EMSCRIPTEN_WEBSOCKET_T l_WebSocketFD = -1;
	//	bool l_bDo_WSS = true;
	//	if (l_bDo_WSS)
	//	{
	//		EmscriptenWebSocketCreateAttributes l_Att = { "wss://funplayapi.pypc.net/api/agent",0,EM_TRUE };
	//		printf("connect to wss://echo.websocket.events/.ws");
	//		l_WebSocketFD = emscripten_websocket_new(&l_Att);
	//	}
	//	else
	//	{
	//		EmscriptenWebSocketCreateAttributes l_Att = {"ws://sitapi.diresoft.net/api/agent",0,EM_TRUE};
	//		l_WebSocketFD = emscripten_websocket_new(&l_Att);
	//	}
	//	emscripten_websocket_set_onopen_callback(l_WebSocketFD, 0, onopen);
	//	emscripten_websocket_set_onerror_callback(l_WebSocketFD, 0, onerror);
	//	emscripten_websocket_set_onclose_callback(l_WebSocketFD, 0, onclose);
	//	emscripten_websocket_set_onmessage_callback(l_WebSocketFD, 0, onmessage);
	//}

	//struct sockaddr_in addr;
	//int res;
	//sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	//if (sockfd == -1) 
	//{
	//	printf("cannot create socket\n");
	//	exit(EXIT_FAILURE);
	//}
	//printf("socket created\n");
	//fcntl(sockfd, F_SETFL, O_NONBLOCK);

	//memset(&addr, 0, sizeof(addr));
	//addr.sin_family = AF_INET;
	//addr.sin_port = 2978;
	//if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) != 1) 
	//{
	//	printf("inet_pton failed\n");
	//	finish(EXIT_FAILURE);
	//}
	//printf("inet_pton ok\n");
	//res = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
	//if (res == -1 && errno != EINPROGRESS) 
	//{
	//	printf("connect failed");
	//	finish(EXIT_FAILURE);
	//}
	//printf("connected\n");
	//linker command line for openglES2
	//building without any flags gives ability to target WebGL 1.0 support via GLES2 headers.No emulation.This is practically core GLES2.
	//building with - s LEGACY_GL_EMULATION = 1 gives(a very limited set of) desktop GL emulation
	//building with - s FULL_ES2 = 1 gives WebGL 1 via core GLES2 headers + emulation to get some missing GLES 2 features, most notably client side rendering support.The flags - s LEGACY_GL_EMULATION = 1 and -s FULL_ES2 = 1 are mutually orthogonal since it's not sensible to target both desktop GL and GLES at the same time.
	//building with - s USE_WEBGL2 = 1 enables targeting core WebGL 2.0 via GLES 3 headers.No emulation of GLES3 features that don't map directly. Having this flag does not yet mean that one will initialize a WebGL 2 context, or that WebGL 2 context will be required by the browser to run the generated build, but the application is capable of then initializing either aWebGL1 or WebGL 2 (GLES2 or GLES3) context. Once browsers ship stable versions of WebGL2, we'll likely make this flag default to 1 instead of current 0.
	//building with - s USE_WEBGL2 = 1 - s FULL_ES3 = 1 enables creating WebGL 2 contexts, and also adds emulation for some GLES3 features that we can emulate(mainly buffer mapping).
	//https://github.com/kripken/emscripten/issues/462
	//--preload-files
	//http://kripken.github.io/emscripten-site/docs/getting_started/Tutorial.html
	//embed-file
	//https://kripken.github.io/emscripten-site/docs/porting/files/packaging_files.html
	//exten max memory
	//http://www.cnblogs.com/ppgeneve/p/5085274.html
	FMLog::Init();
	int CANVANS_WIDTH = EMSDK::EMSDK_GetBrowserWidth();
	int CANVANS_HEIGHT = EMSDK::EMSDK_GetBrowserHeight();
	//CANVANS_WIDTH = 720;
	//CANVANS_HEIGHT = 1280;
	FMLOG("BrowserW:%d,BrowserH:%d", CANVANS_WIDTH, CANVANS_HEIGHT);
	//char cwd[PATH_MAX];
	//if (getcwd(cwd, sizeof(cwd)) != NULL)
	//{
	//	printf("Directory:%s\n",cwd);
	//}
	cGameApp::CreateDefaultOpenGLRender();
	cGameApp::m_spOpenGLRender->m_vViewPortSize.x = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.x = 0;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.y = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.y = 0;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.z = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.z = CANVANS_WIDTH;
	cGameApp::m_spOpenGLRender->m_vViewPortSize.w = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.w = CANVANS_HEIGHT;
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return -1;
	}
	//https://www.libsdl.org/release/SDL-1.2.15/docs/html/guidevideoopengl.html
	//http://lazyfoo.net/SDL_tutorials/lesson04/index.php
	SDL_Surface* l_pSurf_Display = nullptr;
	FMLOG("do SDL_SetVideoMode\n");
	if ((l_pSurf_Display = SDL_SetVideoMode(CANVANS_WIDTH, CANVANS_HEIGHT, 32, SDL_OPENGL)) == NULL)
	{
		FMLOG("do SDL_SetVideoMode failed\n");
		return -1;
	}
	FMLOG("do SDL_SetVideoMode okay\n");
	//cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution(800,600, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	//g_pPreLoadFromInternet = new cPreLoadFromInternet();
	//bool	l_bDurningPreload = g_pPreLoadFromInternet->Init("assets/PreloadResource.xml");
	//EMSCRIPTEN_WEBSOCKET_T ws;
	if (l_pSurf_Display)
	{
		//if (emscripten_websocket_is_supported()) 
		//{
		//	printf("emscripten_websocket_is_supported okay\n");
		//	EmscriptenWebSocketCreateAttributes ws_attrs = 
		//	{
		//		"127.0.0.1:2978",
		//		NULL,
		//		EM_TRUE
		//	};
		//	ws = emscripten_websocket_new(&ws_attrs);
		//	emscripten_websocket_set_onopen_callback(ws, NULL, onopen);
		//	emscripten_websocket_set_onerror_callback(ws, NULL, onerror);
		//	emscripten_websocket_set_onclose_callback(ws, NULL, onclose);
		//	emscripten_websocket_set_onmessage_callback(ws, NULL, onmessage);
		//}
		//else
		//{
		//	printf("emscripten_websocket_is_supported failed\n");
		//}
		cGameApp::m_sbDebugFunctionWorking = false;
		g_pGameApp = new cFishApp(cGameApp::m_spOpenGLRender->m_vGameResolution, Vector2(cGameApp::m_spOpenGLRender->m_vViewPortSize.Width(), cGameApp::m_spOpenGLRender->m_vViewPortSize.Height()));
		cGameApp::m_spOpenGLRender->m_vGameResolution.x = 720;
		cGameApp::m_spOpenGLRender->m_vGameResolution.y = 1280;
		cGameApp::m_spOpenGLRender->SetAcceptRationWithGameresolution(CANVANS_HEIGHT, CANVANS_WIDTH, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x, (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y);
		//g_p0814adds = new cBaseImage("0814a.dds");
		g_pGameApp->Init();
		emscripten_set_main_loop(&Loop, 0, 1);
	}
	return 0;
}