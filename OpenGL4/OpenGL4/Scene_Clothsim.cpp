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

	m_FloorPlane = new GameObject({0,-2,-10});
	m_FloorPlane->SetMesh(StaticMesh::Plane);
	m_FloorPlane->SetShader("SingleTexture.vert", "SingleTexture.frag");
	m_FloorPlane->SetActiveTextures({TextureLoader::LoadTexture("Grass.jpg")});
	m_FloorPlane->SetScale({ 10,1,10 });

	m_Cloth = new Cloth;
}

void Scene_Clothsim::Update()
{
	m_ElapsedTime += Statics::DeltaTime;
	Statics::SceneCamera.Movement_Capture();
	Statics::SceneCamera.Movement();

	if (m_Cloth)
		m_Cloth->Update();
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
}

void Scene_Clothsim::Draw()
{
	BindImGUI();
	FrameBuffer::GetInstance().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Skybox::GetInstance().Draw();

	if (m_FloorPlane)
		m_FloorPlane->Draw();

	if (m_Cloth)
		m_Cloth->Draw();

	HandleDebugTools();

	FrameBuffer::GetInstance().Unbind();
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
	ImGui::Begin("Window Name");
	ImGui::Text("Helo World!");
	ImGui::End();
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
