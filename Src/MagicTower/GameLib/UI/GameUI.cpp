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

int FirstIdx = 0;

int SecondIdx = 0;

void	TreeNodeDragDropTest();

void ImGuiUpdateTesting()
{
	//https://github.com/ocornut/imgui/issues/5885
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
	TreeNodeDragDropTest();
	// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;
		ImVec4 color_multipler(1, 1, 1, 1);
		ImGui::PushFont(g_pFont_title);
		ImGui::Begin("Chinese Text Example");
		ImGui::Text("您好，世界！");  // This is "Hello, World!" in Traditional Chinese
		ImGui::Image((void*)2, ImVec2((float)300, (float)300), ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), color_multipler);
		ImGui::End();


		ImGui::Begin("你好");                          // Create a window called "Hello, world!" and append into it.
		if (ImGui::TreeNode("options"))
		{
			for (int i = 0; i < 5; i++)
			{
				if (i == 0)
					ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::TreeNode((void*)(intptr_t)i, "功能 %d", i))
				{
					ImGui::Text("icon");
					ImGui::SameLine();
					if (ImGui::SmallButton("按钮1")) 
					{
						FirstIdx = i; SecondIdx = 0; 
					}
					ImGui::Text("icon");
					ImGui::SameLine();
					if (ImGui::SmallButton("按钮2"))
					{
						FirstIdx = i; SecondIdx = 1; 
					}
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}
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



// Structure to hold our tree item data
struct TreeNodeData 
{
	const char* name;
	bool is_dragging = false;
};

// Array of tree nodes
TreeNodeData treeNodes[] =
{
	{ "Node 1" },
	{ "Node 2" },
	{ "Node 3" }
};

void	TreeNodeDragDropTest()
{
	// Root tree node
	if (ImGui::TreeNode("Root Node"))
	{
		for (int i = 0; i < IM_ARRAYSIZE(treeNodes); i++)
		{
			TreeNodeData& node = treeNodes[i];

			// Create a tree node for each item
			if (ImGui::TreeNode(node.name))
			{

				// Drag Source
				if (ImGui::BeginDragDropSource())
				{
					ImGui::SetDragDropPayload("DND_DEMO", &i, sizeof(int));  // Set payload to index
					ImGui::Text("Dragging %s", node.name);
					node.is_dragging = true;
					ImGui::EndDragDropSource();
				}
				else
				{
					node.is_dragging = false;
				}

				// Drag Target
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO"))
					{
						IM_ASSERT(payload->DataSize == sizeof(int));
						int sourceIndex = *(const int*)payload->Data;
						std::swap(treeNodes[i], treeNodes[sourceIndex]);  // Swap nodes on drop
					}
					ImGui::EndDragDropTarget();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
}


//// Helper function to display JSON in ImGui
//void RenderJson(const nlohmann::json& jsonData, const std::string& name = "root") 
//{
//	if (jsonData.is_object()) {
//		// JSON Object (Dictionary)
//		if (ImGui::TreeNode(name.c_str())) {
//			for (auto& item : jsonData.items()) {
//				RenderJson(item.value(), item.key());
//			}
//			ImGui::TreePop();
//		}
//	}
//	else if (jsonData.is_array()) {
//		// JSON Array
//		if (ImGui::TreeNode((name + " (array)").c_str())) {
//			for (size_t i = 0; i < jsonData.size(); ++i) {
//				RenderJson(jsonData[i], "[" + std::to_string(i) + "]");
//			}
//			ImGui::TreePop();
//		}
//	}
//	else {
//		// JSON Value (String, Number, Boolean, etc.)
//		ImGui::Text("%s: %s", name.c_str(), jsonData.dump().c_str());
//	}
//}
