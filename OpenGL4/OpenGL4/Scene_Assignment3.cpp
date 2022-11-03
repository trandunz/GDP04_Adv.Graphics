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
	
	m_AssimpObject = new GameObject({ 0,-25, 0 });
	m_AssimpObject->SetSkinnedMesh(m_AnimatedModel);
	m_AssimpObject->SetShader("SkeletalAnimation.vert", "BlinnFong3D.frag");

	m_Plane = new GameObject({ 0,-25, 0 });
	m_Plane->SetMesh(StaticMesh::Triangle_Quad);
	m_Plane->SetShader("Normals3D.vert", "BlinnFong3D.frag");
	m_Plane->SetActiveTextures({TextureLoader::LoadTexture("Grass.jpg")});
	m_Plane->Rotate({ 1,0,0 }, -90);
	m_Plane->Scale({ 20,20,1 });

	Statics::SceneCamera.SetPitch(-90);
	Statics::SceneCamera.SetYaw(90);
}

void Scene_Assignment3::Update()
{
	m_ElapsedTime += Statics::DeltaTime;
	if (!Statics::ActiveCursor)
	{
		//Statics::SceneCamera.Movement_Capture();
		Statics::SceneCamera.Movement();
	}

	if (m_AssimpObject && Statics::ActiveCursor)
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
	//if (!Statics::ActiveCursor)
	//	Statics::SceneCamera.MouseLook({ xpos, ypos });
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
