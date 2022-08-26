#include "Scene_Assessment1.h"

Scene_Assessment1::Scene_Assessment1()
{
	Start();
}

Scene_Assessment1::~Scene_Assessment1()
{
	skyboxRef = nullptr;

	if (lightManager)
		delete lightManager;
	lightManager = nullptr;

	if (TerrainMesh)
		delete TerrainMesh;
	TerrainMesh = nullptr;

	if (ModelMesh)
		delete ModelMesh;
	ModelMesh = nullptr;

	if (CubeMesh)
		delete CubeMesh;
	CubeMesh = nullptr;

	if (SphereMesh)
		delete SphereMesh;
	SphereMesh = nullptr;

	if (SceneCamera)
		delete SceneCamera;
	SceneCamera = nullptr;

	if (ModelObject)
		delete ModelObject;
	ModelObject = nullptr;
}

void Scene_Assessment1::Start()
{
	Noise::CreateNoiseRAW("RandomNoise", 512, 512);

	ModelMesh = new Mesh("LowPoly/Cross.obj");
	SphereMesh = new Mesh(SHAPE::SPHERE, GL_CCW);
	CubeMesh = new Mesh(SHAPE::CUBE, GL_CCW);

	SceneCamera = new Camera({ 0,0,5 });
	ModelObject = new GameObject(*SceneCamera, { 0,0,0 });

	lightManager = new LightManager(*SceneCamera);
	DirectionalLight sun{};
	sun.Direction = { -1,-1,0 };
	lightManager->CreateDirectionalLight(sun);

	TextureLoader::Init({
		"World.jpg",
		"Grass.jpg"
		});

	ModelObject->SetActiveTextures({ TextureLoader::LoadTexture("LowPoly/Cross.png") });
	ModelObject->SetMesh(ModelMesh);
	ModelObject->SetShader("SingleTexture.vert", "SingleTexture.frag");
	ModelObject->SetLightManager(*lightManager);
	ModelObject->SetScale({ 0.01f,0.01f,0.01f });

	skyboxRef = &Skybox::GetInstance(SceneCamera, TextureLoader::LoadCubemap(
		{
			"MountainOutpost/Right.jpg",
			"MountainOutpost/Left.jpg",
			"MountainOutpost/Up.jpg",
			"MountainOutpost/Down.jpg",
			"MountainOutpost/Back.jpg",
			"MountainOutpost/Front.jpg",
		}
	));

	TerrainMesh = new Terrain(*SceneCamera, "RandomNoise",".RAW");
	TerrainMesh->SetLightManager(*lightManager);
	TerrainMesh->SetActiveTextures(
		{
			TextureLoader::LoadTexture("Sand.jpg"),
			TextureLoader::LoadTexture("Dirt.jpg"),
			TextureLoader::LoadTexture("Grass.jpg"),
			TextureLoader::LoadTexture("Snow.jpg")
		});
	TerrainMesh->SetScale({ 0.05f,0.05f,0.05f });
	TerrainMesh->SetTranslation({ 0.0f,-20.0f,0.0f });
}

void Scene_Assessment1::Update()
{
	if (SceneCamera)
	{
		SceneCamera->Movement_Capture();
		SceneCamera->Movement();
	}

	if (ModelObject)
	{
		ModelObject->Rotate({ 0,1,0 }, 1000 * glm::radians(Statics::DeltaTime));
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
				BlackBars = !BlackBars;
			}
			if (key.first == GLFW_KEY_X)
			{
				key.second = false;
				Statics::StencilTest = !Statics::StencilTest;
			}
		}
	}
}

void Scene_Assessment1::MouseEvents(double& xpos, double& ypos)
{
	if (SceneCamera)
	{
		SceneCamera->MouseLook({ xpos, ypos });
	}
}

void Scene_Assessment1::Draw()
{
	if (BlackBars)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 50, Statics::WindowSize.x, Statics::WindowSize.y - 100);
	}

	skyboxRef->Draw();

	if (ModelObject)
		ModelObject->Draw();

	if (TerrainMesh)
		TerrainMesh->Draw();

	if (BlackBars)
	{
		glDisable(GL_SCISSOR_TEST);
	}
}
