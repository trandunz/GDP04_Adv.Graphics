// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Scene_Assessment1.cpp 
// Description : Scene_Assessment1 Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Scene_Assessment1.h"
#include "TextureLoader.h"
#include "Noise.h"
#include "StaticMesh.h"
#include "FrameBuffer.h"

Scene_Assessment1::Scene_Assessment1()
{
	Start();
}

Scene_Assessment1::~Scene_Assessment1()
{
	if (m_FlatQuad)
		delete m_FlatQuad;
	m_FlatQuad = nullptr;

	if (m_LeftQuad)
		delete m_LeftQuad;
	m_LeftQuad = nullptr;

	if (m_RightQuad)
		delete m_RightQuad;
	m_RightQuad = nullptr;

	if (m_MousePickSphere)
		delete m_MousePickSphere;
	m_MousePickSphere = nullptr;

	if (m_TerrainFollowingSphere)
		delete m_TerrainFollowingSphere;
	m_TerrainFollowingSphere = nullptr;

	if (m_LitTerrain)
		delete m_LitTerrain;
	m_LitTerrain = nullptr;

	if (m_NoiseTerrain)
		delete m_NoiseTerrain;
	m_NoiseTerrain = nullptr;

	if (m_ModelMesh)
		delete m_ModelMesh;
	m_ModelMesh = nullptr;

	if (m_ModelObject)
		delete m_ModelObject;
	m_ModelObject = nullptr;

	if (m_MossQuad)
		delete m_MossQuad;
	m_MossQuad = nullptr;
}

void Scene_Assessment1::Start()
{
	glfwSetInputMode(Statics::RenderWindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	// Preload textures to use
	TextureLoader::Init({
	"Grass.jpg",
	"Sand.jpg",
	"Dirt.jpg",
	"Snow.jpg"
	});

	Noise::CreateNoiseRAW("RandomNoise", 513, 513);
	Noise::CreateNoiseJPG("MossNoise", 513, 513);

	LightManager::GetInstance().CreateDirectionalLight({ { -1,-1,0 } });

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
	Skybox::GetInstance().SetCloudActive(true);

	m_ModelMesh = new Mesh("LowPoly/Cross.obj");

	m_LitTerrain = new Terrain("Basic", ".RAW");
	m_LitTerrain->SetActiveTextures(
		{
			TextureLoader::LoadTexture("Sand.jpg"),
			TextureLoader::LoadTexture("Dirt.jpg"),
			TextureLoader::LoadTexture("Grass.jpg"),
			TextureLoader::LoadTexture("Snow.jpg")
		});
	m_LitTerrain->SetScale({ 0.05f,0.05f,0.05f });
	m_LitTerrain->SetTranslation({ 0.0f,-20.0f,0.0f });

	m_NoiseTerrain = new Terrain("RandomNoise", ".RAW");
	m_NoiseTerrain->SetActiveTextures(
		{
			TextureLoader::LoadTexture("Sand.jpg"),
			TextureLoader::LoadTexture("Dirt.jpg"),
			TextureLoader::LoadTexture("Grass.jpg"),
			TextureLoader::LoadTexture("Snow.jpg")
		});
	m_NoiseTerrain->SetScale({ 0.05f,0.05f,0.05f });
	m_NoiseTerrain->SetTranslation({ -25.6,-20.0f,0.0f });

	m_ModelObject = new GameObject;
	m_ModelObject->SetActiveTextures({ TextureLoader::LoadTexture("LowPoly/Cross.png") });
	m_ModelObject->SetMesh(m_ModelMesh);
	m_ModelObject->SetShader("Fog.vert", "Fog.frag");
	m_ModelObject->SetScale({ 0.01f,0.01f,0.01f });

	m_LeftQuad = new GameObject;
	m_LeftQuad->SetActiveTextures({ TextureLoader::LoadTexture("LeftQuad.PNG") });
	m_LeftQuad->SetMesh(StaticMesh::Triangle_Quad);
	m_LeftQuad->SetShader("SingleTexture.vert", "SingleTexture.frag");

	m_RightQuad = new GameObject;
	m_RightQuad->SetActiveTextures({ TextureLoader::LoadTexture("RightQuad.PNG") });
	m_RightQuad->SetMesh(StaticMesh::Triangle_Quad);
	m_RightQuad->SetShader("SingleTexture.vert", "SingleTexture.frag");

	m_FlatQuad = new GameObject;
	m_FlatQuad->SetActiveTextures({ TextureLoader::LoadTexture("Dirt.JPG") });
	m_FlatQuad->SetMesh(StaticMesh::Triangle_Quad);
	m_FlatQuad->SetShader("Fog.vert", "Fog.frag");
	m_FlatQuad->SetRotation({ 1,0,0 }, 90.0f);
	m_FlatQuad->SetScale({ 10.0f,10.0f,10.0f });

	m_MossQuad = new GameObject;
	m_MossQuad->SetActiveTextures(
		{
			TextureLoader::LoadTexture("Dirt.JPG"),
			TextureLoader::LoadTexture("Moss.JPG"),
			TextureLoader::LoadTexture("Heightmaps/MossNoise.JPG")
		});
	m_MossQuad->SetMesh(StaticMesh::Triangle_Quad);
	m_MossQuad->SetShader("Fog.vert", "Perlin_Moss.frag");
	m_MossQuad->SetScale({ 10.0f,10.0f,10.0f });
	m_MossQuad->SetTranslation({ 0, 0.0f, -10.0f });

	m_MousePickSphere = new GameObject;
	m_MousePickSphere->SetMesh(StaticMesh::Sphere);
	m_MousePickSphere->SetShader("Fog.vert", "Fog.frag");
	m_MousePickSphere->SetScale({ 0.5f,0.5f,0.5f });
	m_MousePickSphere->SetTranslation({ 0.0f,0.25f,0.0f });
	m_MousePickSphere->SetStencilOutlineActive(false);
}

void Scene_Assessment1::Update()
{
	m_ElapsedTime += Statics::DeltaTime;
	Statics::SceneCamera.Movement_Capture();
	Statics::SceneCamera.Movement();

	if (m_ModelObject)
		m_ModelObject->Rotate({ 0,1,0 }, 1000 * glm::radians(Statics::DeltaTime));

	HandleMousePickingInteractions();

	if (m_LeftQuad)
	{
		m_LeftQuad->BillboardObjectToCamera(
			(Statics::SceneCamera.GetFront() * 3.0f) - Statics::SceneCamera.GetRight(),
			{ 0.25f ,0.25f,0.25f });
	}
	if (m_RightQuad)
	{
		m_RightQuad->BillboardObjectToCamera(
			(Statics::SceneCamera.GetFront() * 3.0f) + Statics::SceneCamera.GetRight(),
			{ 0.25f ,0.25f,0.25f });
	}
}

void Scene_Assessment1::KeyEvents()
{
	for (auto& key : Statics::Keymap)
	{
		if (key.second)
		{
			if (key.first == GLFW_KEY_Z)
			{
				key.second = false;
				Statics::BlackBars = !Statics::BlackBars;
			}
			if (key.first == GLFW_KEY_X)
			{
				key.second = false;
				Statics::StencilTest = !Statics::StencilTest;
			}
			if (key.first == GLFW_KEY_F)
			{
				key.second = false;
				Statics::Foggy = !Statics::Foggy;
			}
		}
	}
}

void Scene_Assessment1::CursorMoveEvent(double& xpos, double& ypos)
{
	m_CursorPos = { xpos , ypos };

	if (!Statics::ActiveCursor)
		Statics::SceneCamera.MouseLook({ xpos, ypos });
}

void Scene_Assessment1::CursorClickEvent(int button, int action, int mods)
{
}

void Scene_Assessment1::Draw()
{
	FrameBuffer::GetInstance().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (Statics::BlackBars)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 50, Statics::WindowSize.x, Statics::WindowSize.y - 100);
	}

	Skybox::GetInstance().Draw();

	if (m_ModelObject)
		m_ModelObject->Draw();

	if (m_RightQuad)
		m_RightQuad->Draw();

	if (m_LeftQuad)
		m_LeftQuad->Draw();
	
	if (m_FlatQuad)
		m_FlatQuad->Draw();

	if (m_MousePickSphere)
		m_MousePickSphere->Draw();

	if (m_LitTerrain)
		m_LitTerrain->Draw();

	if (m_NoiseTerrain)
		m_NoiseTerrain->Draw();

	if (m_MossQuad)
		m_MossQuad->Draw();


	if (Statics::BlackBars)
		glDisable(GL_SCISSOR_TEST);

	FrameBuffer::GetInstance().Unbind();
	glfwSwapBuffers(Statics::RenderWindow);
}

void Scene_Assessment1::HandleMousePickingInteractions()
{
	if (Statics::ActiveCursor)
	{
		// If mouse is pressed / held
		int mouseLeftState = glfwGetMouseButton(Statics::RenderWindow, GLFW_MOUSE_BUTTON_LEFT);
		if (mouseLeftState == GLFW_PRESS)
		{
			// Create a ray
			Ray cameraRay{};
			cameraRay.origin = Statics::SceneCamera.GetPosition();
			cameraRay.direction = Statics::SceneCamera.GetRayCursorRayDirection(m_CursorPos);

			// Check for intersection with left quad
			if (m_LeftQuad)
			{
				bool intersection = m_LeftQuad->RayIntersection(cameraRay);
				if (intersection)
				{
					Statics::SceneCamera.MoveForward();
				}
			}
			// Check for intersection with right quad
			if (m_RightQuad)
			{
				bool intersection = m_RightQuad->RayIntersection(cameraRay);
				if (intersection)
				{
					Statics::SceneCamera.MoveBackward();
				}
			}
			// Check for intersection with flat quad
			if (m_FlatQuad)
			{
				// Get the hit pos out of ray intersecttion function
				glm::vec3 hitPos{};
				bool intersection = m_FlatQuad->RayIntersection(cameraRay, hitPos);
				if (intersection)
				{
					// Print it to console
					Print("Intersection Location: ", false);
					Print(hitPos);

					// Set the mouse pick sphere position to inttersection point
					hitPos.y += m_MousePickSphere->GetTransform().scale.y / 2.0f;
					m_MousePickSphere->SetTranslation(hitPos);
				}
			}
		}
	}
	
}
