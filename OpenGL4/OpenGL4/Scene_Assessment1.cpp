#include "Scene_Assessment1.h"
#include "TextureLoader.h"
#include "StaticMesh.h"
#include "Noise.h"

Scene_Assessment1::Scene_Assessment1()
{
	Start();
}

Scene_Assessment1::~Scene_Assessment1()
{
	if (m_LeftQuad)
		delete m_LeftQuad;
	m_LeftQuad = nullptr;

	if (m_RightQuad)
		delete m_RightQuad;
	m_RightQuad = nullptr;

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
}

void Scene_Assessment1::Start()
{
	TextureLoader::Init({
	"World.jpg",
	"Grass.jpg"
	});

	Noise::CreateNoiseRAW("RandomNoise", 513, 513);

	LightManager::GetInstance().CreateDirectionalLight({ { -1,-1,0 } });

	Skybox::GetInstance().SetTexture(TextureLoader::LoadCubemap(
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
	m_LitTerrain->SetTranslation({ 12.8f,-20.0f,0.0f });

	m_NoiseTerrain = new Terrain("RandomNoise", ".RAW");
	m_NoiseTerrain->SetActiveTextures(
		{
			TextureLoader::LoadTexture("Sand.jpg"),
			TextureLoader::LoadTexture("Dirt.jpg"),
			TextureLoader::LoadTexture("Grass.jpg"),
			TextureLoader::LoadTexture("Snow.jpg")
		});
	m_NoiseTerrain->SetScale({ 0.05f,0.05f,0.05f });
	m_NoiseTerrain->SetTranslation({ -12.8f,-20.0f,0.0f });

	m_ModelObject = new GameObject;
	m_ModelObject->SetActiveTextures({ TextureLoader::LoadTexture("LowPoly/Cross.png") });
	m_ModelObject->SetMesh(m_ModelMesh);
	m_ModelObject->SetShader("Fog.vert", "Fog.frag");
	m_ModelObject->SetScale({ 0.01f,0.01f,0.01f });

	m_LeftQuad = new GameObject;
	m_LeftQuad->SetActiveTextures({ TextureLoader::LoadTexture("LeftQuad.PNG") });
	m_LeftQuad->SetMesh(StaticMesh::Quad);
	m_LeftQuad->SetShader("SingleTexture.vert", "SingleTexture.frag");
	m_LeftQuad->Scale({ 0.5f ,0.5f,0.5f });

	m_RightQuad = new GameObject;
	m_RightQuad->SetActiveTextures({ TextureLoader::LoadTexture("RightQuad.PNG") });
	m_RightQuad->SetMesh(StaticMesh::Quad);
	m_RightQuad->SetShader("SingleTexture.vert", "SingleTexture.frag");
	m_RightQuad->Scale({ 0.5f ,0.5f,0.5f });
}

void Scene_Assessment1::Update()
{
	Statics::SceneCamera.Movement_Capture();
	Statics::SceneCamera.Movement();

	if (m_ModelObject)
		m_ModelObject->Rotate({ 0,1,0 }, 1000 * glm::radians(Statics::DeltaTime));

	if (m_LeftQuad)
	{
		glm::vec3 position = Statics::SceneCamera.GetPosition() + (Statics::SceneCamera.GetFront() * 4.0f) - Statics::SceneCamera.GetRight();
		m_LeftQuad->SetModel(glm::inverse(glm::lookAt(position, Statics::SceneCamera.GetPosition(), Statics::SceneCamera.GetUp())));
	}

	if (m_RightQuad)
	{
		glm::vec3 position = Statics::SceneCamera.GetPosition() + (Statics::SceneCamera.GetFront() * 4.0f) + Statics::SceneCamera.GetRight();
		m_RightQuad->SetModel(glm::inverse(glm::lookAt(position, Statics::SceneCamera.GetPosition(), Statics::SceneCamera.GetUp())));
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

void Scene_Assessment1::MouseEvents(double& xpos, double& ypos)
{
	if (!Statics::ActiveCursor)
		Statics::SceneCamera.MouseLook({ xpos, ypos });
}

void Scene_Assessment1::Draw()
{
	Skybox::GetInstance().Draw();

	if (m_ModelObject)
		m_ModelObject->Draw();

	if (m_RightQuad)
		m_RightQuad->Draw();

	if (m_LeftQuad)
		m_LeftQuad->Draw();

	if (m_LitTerrain)
		m_LitTerrain->Draw();

	if (m_NoiseTerrain)
		m_NoiseTerrain->Draw();
}
