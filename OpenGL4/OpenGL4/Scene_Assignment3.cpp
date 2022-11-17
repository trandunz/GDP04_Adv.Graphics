#include "Scene_Assignment3.h"
#include "TextureLoader.h"
#include "FrameBuffer.h"
#include "LightManager.h"

Scene_Assignment3::Scene_Assignment3()
{
	InitImGUI();
	Start();
}

Scene_Assignment3::~Scene_Assignment3()
{
	if (m_AnimatedModel)
		delete m_AnimatedModel;
	m_AnimatedModel = nullptr;

	if (m_Plane)
		delete m_Plane;
	m_Plane = nullptr;

	if (m_Player)
		delete m_Player;
	m_Player = nullptr;

	if (m_ParticleSystem)
		delete m_ParticleSystem;
	m_ParticleSystem = nullptr;

	if (m_Bonfire)
		delete m_Bonfire;
	m_Bonfire = nullptr;

	if (m_BonfireModel)
		delete m_BonfireModel;
	m_BonfireModel = nullptr;

	if (m_SmokeSystem)
		delete m_SmokeSystem;
	m_SmokeSystem = nullptr;

	if (m_FountainSystem)
		delete m_FountainSystem;
	m_FountainSystem = nullptr;

	if (m_FountainModel)
		delete m_FountainModel;
	m_FountainModel = nullptr;

	if (m_Fountain)
		delete m_Fountain;
	m_Fountain = nullptr;

	for (auto& firework : m_Fireworks)
	{
		if (firework)
			delete firework;
		firework = nullptr;
	}

	CleanupImGUI();
}

void Scene_Assignment3::Start()
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
			}
	));

	LightManager::GetInstance().CreateDirectionalLight({ {0,-1,0} });

	m_AnimatedModel = new SkinnedMesh("Resources/Models/Dude/Dude.dae");
	m_AnimatedModel->SetCurrentAnimation(0, 30);

	m_FountainModel = new SkinnedMesh("Resources/Models/Fountain/Fountain.fbx");

	m_BonfireModel = new SkinnedMesh("Resources/Models/Bonfire/Bonfire.fbx");

	m_Plane = new GameObject({ 0,-25, -10});
	m_Plane->SetMesh(StaticMesh::Triangle_Quad);
	m_Plane->SetShader("Normals3D.vert", "BlinnFong3D.frag");
	m_Plane->SetActiveTextures({TextureLoader::LoadTexture("Grass.jpg")});
	m_Plane->Rotate({ 1,0,0 }, -90);
	m_Plane->Scale({ 20,20,1 });

	m_Player = new Player({ 0,-25, -10 });
	m_Player->SetSkinnedMesh(m_AnimatedModel);
	m_Player->SetShader("SkeletalAnimation.vert", "BlinnFong3D.frag");
	m_Player->SetGround(m_Plane);

	m_Bonfire = new GameObject({ 0,-25, -15 });
	m_Bonfire->SetSkinnedMesh(m_BonfireModel);
	m_Bonfire->SetShader("Normals3D.vert", "BlinnFong3D.frag");
	m_Bonfire->SetActiveTextures({ TextureLoader::LoadTexture("Bonfire.jpg") });
	m_Bonfire->Rotate({ 1,0,0 }, 90);

	Statics::ActiveCamera->SetPitch(-110);
	Statics::ActiveCamera->SetYaw(90);

	m_ParticleSystem = new ParticleSystem({0,-25,-15 }, 0.1f);
	m_ParticleSystem->SetParticleTexture(TextureLoader::LoadTexture("Flame.png"));
	m_ParticleSystem->SetGravity(false);
	m_ParticleSystem->SetLifetime(0.1f);
	m_ParticleSystem->SetAlphaOverLifetime(0);
	m_ParticleSystem->Init();

	m_SmokeSystem = new ParticleSystem({ 0,-23.5f,-15 }, 3.0f);
	m_SmokeSystem->SetParticleTexture(TextureLoader::LoadTexture("Smoke.png"));
	m_SmokeSystem->SetGravity(false);
	m_SmokeSystem->SetLifetime(3.0f);
	m_SmokeSystem->SetAlphaOverLifetime(0);
	m_SmokeSystem->Init();

	m_FountainSystem = new C_Particle_System({ 0,-23.7f,-5 }, 0.1f);
	m_FountainSystem->SetParticleTexture(TextureLoader::LoadTexture("Water.png"));
	m_FountainSystem->Init();

	m_SnowSystem = new C_Particle_System({ 0,0,-10 }, 0.1f);
	m_SnowSystem->SetParticleTexture(TextureLoader::LoadTexture("Snowflake.png"));
	m_SnowSystem->SetGravity(0.5f);
	m_SnowSystem->YVelocity = 0;
	m_SnowSystem->m_Lifetime = 10;
	m_SnowSystem->EmissionOffset = { rand() % 100, 0, rand() % 100 };
	m_SnowSystem->Init();

	m_Fountain = new GameObject({ 0,-25,-5 });
	m_Fountain->SetSkinnedMesh(m_FountainModel);
	m_Fountain->SetShader("Normals3D.vert", "BlinnFong3D.frag");
	m_Fountain->SetActiveTextures({ TextureLoader::LoadTexture("lambert1_Base_Color.png") });
	m_Fountain->SetScale({ 0.1f,0.1f,0.1f });
}

void Scene_Assignment3::Update()
{
	m_ElapsedTime += Statics::DeltaTime;
	if (!Statics::ActiveCursor)
	{
		Statics::ActiveCamera->Movement_Capture();
		Statics::ActiveCamera->Movement();
	}

	if (m_ParticleSystem)
	{
		m_ParticleSystem->Update();
	}

	if (m_SmokeSystem)
	{
		m_SmokeSystem->Update();
	}

	if (m_FountainSystem)
	{
		m_FountainSystem->Update();
	}

	if (m_SnowSystem)
	{
		m_SnowSystem->Update();
	}

	for (auto& firework : m_Fireworks)
	{
		firework->Update();
	}
		

	if (m_Player)
	{
		m_Player->Update();
	}
}

void Scene_Assignment3::KeyEvents()
{
	for (auto& key : Statics::Keymap)
	{
		if(key.second)
		{
			if (key.first == GLFW_KEY_P)
			{
				m_Fireworks.emplace_back(new Firework({ 0,-25, -10 }));
				key.second = false;
			}
		}
	}
}

void Scene_Assignment3::CursorMoveEvent(double& xpos, double& ypos)
{
	if (!Statics::ActiveCursor)
		Statics::ActiveCamera->MouseLook({ xpos, ypos });
}

void Scene_Assignment3::CursorClickEvent(int button, int action, int mods)
{
}

void Scene_Assignment3::Draw()
{
	FrameBuffer::GetInstance().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Skybox::GetInstance().Draw();

	if (m_Player)
		m_Player->Draw();

	if (m_Plane)
		m_Plane->Draw();

	if (m_Bonfire)
		m_Bonfire->Draw();

	if (m_Fountain)
		m_Fountain->Draw();

	for (auto& firework : m_Fireworks)
		firework->Draw();

	if (m_SmokeSystem)
		m_SmokeSystem->Draw();

	if (m_ParticleSystem)
		m_ParticleSystem->Draw();

	if (m_FountainSystem)
		m_FountainSystem->Draw();

	if (m_SnowSystem)
		m_SnowSystem->Draw();

	FrameBuffer::GetInstance().Unbind();

	BindImGUI();
	HandleDebugTools();
	DrawImGUI();

	glfwSwapBuffers(Statics::RenderWindow);
}

void Scene_Assignment3::InitImGUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	m_Io = &ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Statics::RenderWindow, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void Scene_Assignment3::BindImGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Scene_Assignment3::DrawImGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Scene_Assignment3::CleanupImGUI()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	if (m_Io)
		delete m_Io;
	m_Io = nullptr;
}

void Scene_Assignment3::HandleDebugTools()
{
	ImGui::Begin("Camera Settings");
	ImGui::Checkbox("ThirdPersonCamera", &m_ThirdPersonCamera);
	if (m_Player)
	{
		m_Player->ToggleThirdPersonCamera(m_ThirdPersonCamera);
	}
	ImGui::End();
}
