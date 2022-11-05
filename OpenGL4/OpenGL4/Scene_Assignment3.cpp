#include "Scene_Assignment3.h"
#include "TextureLoader.h"
#include "FrameBuffer.h"
#include "LightManager.h"

Scene_Assignment3::Scene_Assignment3()
{
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

	if (m_AssimpObject)
		delete m_AssimpObject;
	m_AssimpObject = nullptr;

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

	LightManager::GetInstance().CreatePointLight({ {0,10,0} });

	m_AnimatedModel = new SkinnedMesh("Resources/Models/Dude/Dude.dae");
	m_AnimatedModel->SetCurrentAnimation(0, 30);

	m_FountainModel = new SkinnedMesh("Resources/Models/Fountain/Fountain.fbx");

	m_BonfireModel = new SkinnedMesh("Resources/Models/Bonfire/Bonfire.fbx");
	
	m_AssimpObject = new GameObject({ 0,-25, -10 });
	m_AssimpObject->SetSkinnedMesh(m_AnimatedModel);
	m_AssimpObject->SetShader("SkeletalAnimation.vert", "BlinnFong3D.frag");

	m_Plane = new GameObject({ 0,-25, -10});
	m_Plane->SetMesh(StaticMesh::Triangle_Quad);
	m_Plane->SetShader("Normals3D.vert", "BlinnFong3D.frag");
	m_Plane->SetActiveTextures({TextureLoader::LoadTexture("Grass.jpg")});
	m_Plane->Rotate({ 1,0,0 }, -90);
	m_Plane->Scale({ 20,20,1 });

	m_Bonfire = new GameObject({ 0,-25, -15 });
	m_Bonfire->SetSkinnedMesh(m_BonfireModel);
	m_Bonfire->SetShader("Normals3D.vert", "BlinnFong3D.frag");
	m_Bonfire->SetActiveTextures({ TextureLoader::LoadTexture("Bonfire.jpg") });
	m_Bonfire->Rotate({ 1,0,0 }, 90);

	Statics::SceneCamera.SetPitch(-110);
	Statics::SceneCamera.SetYaw(90);

	m_ParticleSystem = new ParticleSystem({0,-25,-15 }, 0.2f);
	m_ParticleSystem->SetShader("PointToQuad.vert", "PointToQuad.geo", "SingleTexture_Coloured.frag");
	m_ParticleSystem->SetParticleTexture(TextureLoader::LoadTexture("Flame.png"));
	m_ParticleSystem->SetGravity(false);
	m_ParticleSystem->SetLifetime(0.1f);
	m_ParticleSystem->SetAlphaOverLifetime(1);

	m_SmokeSystem = new ParticleSystem({ 0,-23.5f,-15 }, 0.1f);
	m_SmokeSystem->SetShader("PointToQuad.vert", "PointToQuad.geo", "SingleTexture_Coloured.frag");
	m_SmokeSystem->SetParticleTexture(TextureLoader::LoadTexture("Smoke.png"));
	m_SmokeSystem->SetGravity(false);
	m_SmokeSystem->SetLifetime(3.0f);
	m_SmokeSystem->SetAlphaOverLifetime(1);

	//m_FountainSystem = new C_Particle_System({ 0,-23.7f,-5 }, 0.1f);
	//m_FountainSystem->SetParticleTexture(TextureLoader::LoadTexture("Water.png"));
	//m_FountainSystem->Init();

	m_SnowSystem = new C_Particle_System({ 0,0,-10 }, 0.1f);
	m_SnowSystem->SetParticleTexture(TextureLoader::LoadTexture("Snowflake.png"));
	m_SnowSystem->SetGravity(1);
	m_SnowSystem->YVelocity = 0;
	m_SnowSystem->m_Lifetime = 10;
	m_SnowSystem->EmissionOffset = { rand() * 1000, 0, rand() * 1000 };
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
		Statics::SceneCamera.Movement_Capture();
		Statics::SceneCamera.Movement();
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
		

	if (m_AssimpObject)
	{
		m_AssimpObject->Movement_WASDEQ();
		
		m_AssimpObject->Update();

		if (glm::length(m_AssimpObject->GetInput()) > 0.1f)
		{
			AlignPlayerWithInput();
			

			if (!m_CharacterMoving)
			{
				m_AnimatedModel->SetCurrentAnimation(31, 50);
			}
			m_CharacterMoving = true;
		}
		else
		{
			if (m_CharacterMoving)
			{
				m_AnimatedModel->SetCurrentAnimation(0, 30);
			}
			m_CharacterMoving = false;
		}
	}
}

void Scene_Assignment3::KeyEvents()
{
}

void Scene_Assignment3::CursorMoveEvent(double& xpos, double& ypos)
{
	if (!Statics::ActiveCursor)
		Statics::SceneCamera.MouseLook({ xpos, ypos });
}

void Scene_Assignment3::CursorClickEvent(int button, int action, int mods)
{
}

void Scene_Assignment3::Draw()
{
	FrameBuffer::GetInstance().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Skybox::GetInstance().Draw();

	if (m_AssimpObject)
		m_AssimpObject->Draw();

	if (m_Plane)
		m_Plane->Draw();

	if (m_Bonfire)
		m_Bonfire->Draw();

	if (m_Fountain)
		m_Fountain->Draw();

	if (m_SmokeSystem)
		m_SmokeSystem->Draw();

	if (m_ParticleSystem)
		m_ParticleSystem->Draw();

	if (m_FountainSystem)
		m_FountainSystem->Draw();

	if (m_SnowSystem)
		m_SnowSystem->Draw();

	FrameBuffer::GetInstance().Unbind();
	glfwSwapBuffers(Statics::RenderWindow);
}

void Scene_Assignment3::AlignPlayerWithInput()
{
	if (m_AssimpObject->GetInput().z < 0 && m_AssimpObject->GetInput().x > 0)
	{
		m_AssimpObject->SetRotation({ 0,1,0 }, -45);
	}
	else if (m_AssimpObject->GetInput().z > 0 && m_AssimpObject->GetInput().x == 0)
	{
		m_AssimpObject->SetRotation({ 0,1,0 }, 180);
	}
	else if (m_AssimpObject->GetInput().z == 0 && m_AssimpObject->GetInput().x > 0)
	{
		m_AssimpObject->SetRotation({ 0,1,0 }, -90);
	}
	else if (m_AssimpObject->GetInput().z == 0 && m_AssimpObject->GetInput().x < 0)
	{
		m_AssimpObject->SetRotation({ 0,1,0 }, 90);
	}
	else if (m_AssimpObject->GetInput().z < 0 && m_AssimpObject->GetInput().x < 0)
	{
		m_AssimpObject->SetRotation({ 0,1,0 }, 45);
	}
	else if (m_AssimpObject->GetInput().z > 0 && m_AssimpObject->GetInput().x < 0)
	{
		m_AssimpObject->SetRotation({ 0,1,0 }, 135);
	}
	else if (m_AssimpObject->GetInput().z < 0 && m_AssimpObject->GetInput().x == 0)
	{
		m_AssimpObject->SetRotation({ 0,1,0 }, 0);
	}
	else if (m_AssimpObject->GetInput().z > 0 && m_AssimpObject->GetInput().x > 0)
	{
		m_AssimpObject->SetRotation({ 0,1,0 }, 225);
	}
	else
	{
		m_AssimpObject->SetRotation({ 0,1,0 }, 0);
	}
}
