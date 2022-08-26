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
	if (m_Terrain)
		delete m_Terrain;
	m_Terrain = nullptr;

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

	Noise::CreateNoiseJPG("RandomNoise", 513, 513);

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

	m_Terrain = new Terrain("RandomNoise", ".RAW");
	m_Terrain->SetActiveTextures(
		{
			TextureLoader::LoadTexture("Sand.jpg"),
			TextureLoader::LoadTexture("Dirt.jpg"),
			TextureLoader::LoadTexture("Grass.jpg"),
			TextureLoader::LoadTexture("Snow.jpg")
		});
	m_Terrain->SetScale({ 0.05f,0.05f,0.05f });
	m_Terrain->SetTranslation({ 0.0f,-20.0f,0.0f });

	m_ModelMesh = new Mesh("LowPoly/Cross.obj");
	m_ModelObject = new GameObject();
	m_ModelObject->SetActiveTextures({ TextureLoader::LoadTexture("LowPoly/Cross.png") });
	m_ModelObject->SetMesh(m_ModelMesh);
	m_ModelObject->SetShader("SingleTexture.vert", "SingleTexture.frag");
	m_ModelObject->SetScale({ 0.01f,0.01f,0.01f });
}

void Scene_Assessment1::Update()
{
	Statics::SceneCamera.Movement_Capture();
	Statics::SceneCamera.Movement();

	if (m_ModelObject)
		m_ModelObject->Rotate({ 0,1,0 }, 1000 * glm::radians(Statics::DeltaTime));
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
	Statics::SceneCamera.MouseLook({ xpos, ypos });
}

void Scene_Assessment1::Draw()
{
	Skybox::GetInstance().Draw();

	if (m_ModelObject)
		m_ModelObject->Draw();

	if (m_Terrain)
		m_Terrain->Draw();
}
