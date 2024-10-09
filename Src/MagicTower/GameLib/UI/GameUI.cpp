#include "stdafx.h"
#include "../FileNameDefine.h"
#include "FingerMovingDirection.h"
#include "GameUI.h"
#include "../FileNameDefine.h"
#include "../../../../FM79979/Src/FM79979Engine/imgui/imgui.h"
#include "../../../../FM79979/Src/FM79979Engine/imgui/ImGuiRender.h"


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

ImFont* g_pFont_title = nullptr;
ImFont* g_pFont_title2 = nullptr;
void	ImGuiInit()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	AddImGuiFonts({ "NotoSansTC-VariableFont_wght.ttf","Chinese.ttf" }, { 18,23 });
	g_pFont_title = GetFontByFileName("NotoSansTC-VariableFont_wght.ttf");
	g_pFont_title2 = GetFontByFileName("Chinese.ttf");
#ifdef WIN32
	ImGui_ImplWin32_InitForOpenGL(cGameApp::m_spOpenGLRender->m_Handle);
#endif
	ImGui_ImplOpenGL3_Init(nullptr);

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
}

void ImGuiUpdateTesting()
{
	auto io = ImGui::GetIO();

#ifdef WASM
	//io.DisplaySize = ImVec2(static_cast<float>(l_iViewportWidth), static_cast<float>(l_iViewportHeight));
#endif
	bool show_demo_window = true;
	bool show_another_window = false;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#ifdef WIN32
	ImGui_ImplWin32_NewFrame();
#elif defined(WASM)
	ImGui_ImplSDL2_NewFrame();
#endif
	ImGui::NewFrame();
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::PushFont(g_pFont_title);
		ImGui::Begin("Chinese Text Example");
		ImGui::Text("您好，世界！");  // This is "Hello, World!" in Traditional Chinese
		ImGui::End();


		ImGui::Begin("你好");                          // Create a window called "Hello, world!" and append into it.
		ImGui::PopFont();
		ImGui::Text("您好，世界！");               // Display some text (you can use a format strings too)
		ImGui::PushFont(g_pFont_title2);
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);
		ImGui::PopFont();
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}
	// Rendering
	ImGui::Render();
}

void ImGuiRenderTesting()
{
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void	ImGuiShutDown()
{
#ifdef WIN32
	ImGui_ImplWin32_Shutdown();
#endif
}