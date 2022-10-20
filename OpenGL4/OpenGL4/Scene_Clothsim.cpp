// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Scene_Clothsim.cpp 
// Description : Scene_Clothsim Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Scene_Clothsim.h"
#include "FrameBuffer.h"
#include "Skybox.h"
#include "TextureLoader.h"

Scene_Clothsim::Scene_Clothsim()
{
	InitImGUI();
	Start();
}

Scene_Clothsim::~Scene_Clothsim()
{
	CleanupImGUI();

	if (m_Cloth)
		delete m_Cloth;
	m_Cloth = nullptr;

	if (m_FloorPlane)
		delete m_FloorPlane;
	m_FloorPlane = nullptr;
}

void Scene_Clothsim::Start()
{
	Skybox::GetInstance().SetTexture(
		TextureLoader::LoadCubemap(
			{
				"MountainOutpost/Right.jpg",
				"MountainOutpost/Left.jpg",
				"MountainOutpost/Up.jpg",
				"MountainOutpost/Down.jpg",
				"MountainOutpost/Back.jpg",
				"MountainOutpost/Front.jpg",
			}));

	//m_FloorPlane = new GameObject({0,-2,-10});
	//m_FloorPlane->SetMesh(StaticMesh::Plane);
	//m_FloorPlane->SetShader("SingleTexture.vert", "SingleTexture.frag");
	//m_FloorPlane->SetActiveTextures({TextureLoader::LoadTexture("Grass.jpg")});
	//m_FloorPlane->SetScale({ 10,1,10 });

	m_Cloth = new Cloth((unsigned)m_ClothWidth, (unsigned)m_ClothLength, 1.0f, {-5,5,-20});
}

void Scene_Clothsim::Update()
{
	m_ElapsedTime += Statics::DeltaTime;
	Statics::SceneCamera.Movement_Capture();
	Statics::SceneCamera.Movement();

	if (m_Cloth)
	{
		m_Cloth->SetHookCount(m_HookCount);
		m_Cloth->SetRingSpacing(m_HookDistance);
		m_Cloth->SetElasticity(m_Stiffness);
		m_Cloth->SetWidth((unsigned)m_ClothWidth);
		m_Cloth->SetHeight((unsigned)m_ClothLength);
		m_Cloth->SetRingSpacing(m_HookDistance);
		m_Cloth->SetWindDirection(m_WindDirection);
		m_Cloth->SetWindStrength(m_WindStrength);
		m_Cloth->SetDebugDraw(m_DebugDraw);
		m_Cloth->Update();
	}	
}

void Scene_Clothsim::KeyEvents()
{
}

void Scene_Clothsim::CursorMoveEvent(double& xpos, double& ypos)
{
	m_CursorPos = { xpos , ypos };

	if (!Statics::ActiveCursor)
		Statics::SceneCamera.MouseLook({ xpos, ypos });
}

void Scene_Clothsim::CursorClickEvent(int button, int action, int mods)
{
	if (m_Cloth)
	{

	}
}

void Scene_Clothsim::Draw()
{
	FrameBuffer::GetInstance().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Skybox::GetInstance().Draw();

	if (m_FloorPlane)
		m_FloorPlane->Draw();

	if (m_WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (m_Cloth)
		m_Cloth->Draw();

	if (m_WireFrame)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	FrameBuffer::GetInstance().Unbind();

	BindImGUI();
	HandleDebugTools();
	DrawImGUI();

	glfwSwapBuffers(Statics::RenderWindow);
}

void Scene_Clothsim::InitImGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_Io = &ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Statics::RenderWindow, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void Scene_Clothsim::BindImGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Scene_Clothsim::HandleDebugTools()
{
	if (m_Cloth)
	{
		ImGui::Begin("Cloth Settings");

		ImGui::Text("General Controls:");
		ImGui::Checkbox("Wireframe Mode", &m_WireFrame);
		ImGui::Checkbox("Draw Points", &m_DebugDraw);
		ImGui::Combo("Mouse Mode", &m_SelectedMouseMode, &m_MouseModeItems[0], IM_ARRAYSIZE(m_MouseModeItems));
		ImGui::Text("Cloth Shape:");
		ImGui::SliderFloat("Cloth Length", &m_ClothLength, 2.0f, 20.0f);
		ImGui::SliderFloat("Cloth Width", &m_ClothWidth, 2.0f, 20.0f);
		ImGui::SliderInt("Number Of Hooks", &m_HookCount, 0, (int)m_ClothWidth);
		ImGui::SliderFloat("Hook Distance", &m_HookDistance, 0.5f, 1.0f);
		ImGui::SliderFloat("Cloth Stiffness", &m_Stiffness, 100.0f, 1000.0f);

		if(ImGui::Button("Reset Cloth"))
		{
			m_ClothLength = 20.0f;
			m_ClothWidth = 20.0f;
			m_HookCount = (int)m_ClothWidth;
			m_HookDistance = 1.0f;
			m_Stiffness = 500.0f;
		}
		ImGui::Text("Object Interation:");
		ImGui::Combo("Selected Object: ", &m_SelectedCollision, &m_CollisionItems[0], IM_ARRAYSIZE(m_CollisionItems));
		ImGui::Text("Wind:");
		ImGui::SliderFloat("Wind Direction (x)", &m_WindDirection.x, 0.0f, 1.0f);
		ImGui::SliderFloat("Wind Direction (y)", &m_WindDirection.y, 0.0f, 1.0f);
		ImGui::SliderFloat("Wind Direction (z)", &m_WindDirection.z, 0.0f, 1.0f);
		ImGui::SliderFloat("Wind Strength:", &m_WindStrength, 0.0f, 25.0f);

		if(ImGui::Button("Reset Wind"))
		{
			m_WindDirection = {};
			m_WindStrength = 0.0f;
		}

		ImGui::End();
	}
}

void Scene_Clothsim::DrawImGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Scene_Clothsim::CleanupImGUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	if (m_Io)
		delete m_Io;
	m_Io = nullptr;
}
