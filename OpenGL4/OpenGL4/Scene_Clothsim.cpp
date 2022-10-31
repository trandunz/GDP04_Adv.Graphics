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
#include "Statics.h"

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

	if (m_CollisionSphere)
		delete m_CollisionSphere;
	m_CollisionSphere = nullptr;
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

	m_FloorPlane = new GameObject({0,-15,-30});
	m_FloorPlane->SetMesh(StaticMesh::Plane);
	m_FloorPlane->SetShader("SingleTexture.vert", "SingleTexture.frag");
	m_FloorPlane->SetActiveTextures({TextureLoader::LoadTexture("Grass.jpg")});
	m_FloorPlane->SetScale({ 30,1,30 });

	m_CollisionSphere = new GameObject({ 0,0,-10 });
	m_CollisionSphere->SetMesh(StaticMesh::Sphere);
	m_CollisionSphere->SetShader("SingleTexture.vert", "SingleTexture.frag");
	m_CollisionSphere->SetActiveTextures({ TextureLoader::LoadTexture("Sand.jpg") });
	m_CollisionSphere->SetScale({ 5,5,5 });
	m_CollisionSphere->SetCollider(new Collider({ 0,-15,0 }, 2.5f));

	m_Cloth = new Cloth((unsigned)m_ClothWidth, (unsigned)m_ClothLength, 1.0f, {-m_ClothWidth / 2,10,-30 });
	m_Cloth->SetGround(*m_FloorPlane);
}

void Scene_Clothsim::Update()
{
	m_ElapsedTime += Statics::DeltaTime;
	Statics::SceneCamera.Movement_Capture();
	Statics::SceneCamera.Movement();

	if (m_CollisionSphere)
	{
		m_CollisionSphere->SetTranslation(glm::vec3{ 0,0,(sin((float)glfwGetTime() / 2) * 20) - 30 });
		m_CollisionSphere->Update();
	}
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
		m_Cloth->CheckCollision(m_CollisionSphere->GetCollider());
		m_Cloth->Update();
	}	
}

void Scene_Clothsim::KeyEvents()
{
}

void Scene_Clothsim::CursorMoveEvent(double& xpos, double& ypos)
{
	if (!Statics::ActiveCursor)
		Statics::SceneCamera.MouseLook({ xpos, ypos });
}

void Scene_Clothsim::CursorClickEvent(int button, int action, int mods)
{
}

void Scene_Clothsim::Draw()
{
	FrameBuffer::GetInstance().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Skybox::GetInstance().Draw();

	if (m_FloorPlane)
		m_FloorPlane->Draw();

	if (m_CollisionSphere)
		m_CollisionSphere->Draw();

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
		if (ImGui::Button("Reset Simulation"))
		{
			m_ClothLength = 20.0f;
			m_ClothWidth = 20.0f;
			m_HookCount = (int)m_ClothWidth;
			m_HookDistance = 1.0f;
			m_Stiffness = 500.0f;

			delete m_Cloth;
			m_Cloth = nullptr;
			m_Cloth = new Cloth((unsigned)m_ClothWidth, (unsigned)m_ClothLength, 1.0f, { -m_ClothWidth / 2,10,-30 });
		}

		ImGui::Text("General Controls:");
		ImGui::Checkbox("Wireframe Mode", &m_WireFrame);
		ImGui::Checkbox("Draw Points", &m_DebugDraw);
		ImGui::Combo("Mouse Mode", &m_SelectedMouseMode, &m_MouseModeItems[0], IM_ARRAYSIZE(m_MouseModeItems));
		UpdateClothInteractionType();
		ImGui::Text("Cloth Shape:");
		ImGui::SliderInt("Cloth Length", &m_ClothLength, 2.0f, 40.0f);
		ImGui::SliderInt("Cloth Width", &m_ClothWidth, 2.0f, 40.0f);
		m_HookCount = m_Cloth->GetHookCount();
		ImGui::SliderInt("Number Of Hooks", &m_HookCount, 0, (int)m_ClothWidth);
		ImGui::SliderFloat("Hook Distance", &m_HookDistance, 0.5f, 1.0f);
		ImGui::SliderFloat("Cloth Stiffness", &m_Stiffness, 100.0f, 1000.0f);

		if (ImGui::Button("Reset Cloth"))
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

		if (ImGui::Button("Reset Wind"))
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

void Scene_Clothsim::UpdateClothInteractionType()
{
	switch (m_SelectedMouseMode)
	{
	case 0:
	{
		m_Cloth->InteractionType = Cloth::INTERACTIONTYPE::PULL;
		break;
	}
	case 1:
	{
		m_Cloth->InteractionType = Cloth::INTERACTIONTYPE::PUSH;
		break;
	}
	default:
	{
		m_Cloth->InteractionType = Cloth::INTERACTIONTYPE::UNASSIGNED;
		break;
	}
	}
}
