#include "Scene_Assessment2.h"
#include "TextureLoader.h"

Scene_Assessment2::Scene_Assessment2()
{
	Start();
}

Scene_Assessment2::~Scene_Assessment2()
{
	if (m_GeoStar)
	{
		delete m_GeoStar;
		m_GeoStar = nullptr;
	}
	if (m_NormalsSphere)
	{
		delete m_NormalsSphere;
		m_NormalsSphere = nullptr;
	}
	if (m_AsymmetricModel)
	{
		delete m_AsymmetricModel;
		m_AsymmetricModel = nullptr;
	}
	if (m_ExplodingObject)
	{
		delete m_ExplodingObject;
		m_ExplodingObject = nullptr;
	}
}

void Scene_Assessment2::Start()
{
	m_AsymmetricModel = new Mesh("LowPoly/Cross.obj");

	m_GeoStar = new GameObject();
	m_GeoStar->SetMesh(StaticMesh::Point);
	m_GeoStar->SetShader("PositionOnly.vert", "PointToStar.geo", "SingleTexture.frag");
	m_GeoStar->SetTranslation({ 0,0,-5.0f });
	m_GeoStar->SetActiveTextures({ TextureLoader::LoadTexture("Snow.jpg") });

	m_NormalsSphere = new GameObject();
	m_NormalsSphere->SetMesh(StaticMesh::Sphere);
	m_NormalsSphere->SetShader("SingleTexture.vert", "SingleTexture.frag");
	m_NormalsSphere->SetTranslation({ 0,0,-2.0f });
	m_NormalsSphere->SetActiveTextures({ TextureLoader::LoadTexture("Dirt.jpg") });
	m_NormalsSphere->SetShowNormals(true);

	m_ExplodingObject = new GameObject();
	m_ExplodingObject->SetMesh(m_AsymmetricModel);
	m_ExplodingObject->SetShader("Normals3D.vert", "Explode.geo", "SingleTexture.frag");
	m_ExplodingObject->SetTranslation({ 3,0,-2.0f });
	m_ExplodingObject->SetActiveTextures({ TextureLoader::LoadTexture("LowPoly/Cross.png") });
	m_ExplodingObject->SetScale({ 0.01f, 0.01f, 0.01f });
}

void Scene_Assessment2::Update()
{
	m_ElapsedTime += Statics::DeltaTime;
	Statics::SceneCamera.Movement_Capture();
	Statics::SceneCamera.Movement();
}

void Scene_Assessment2::KeyEvents()
{
	for (auto& key : Statics::Keymap)
	{
		if (key.second)
		{
		}
	}
}

void Scene_Assessment2::CursorMoveEvent(double& xpos, double& ypos)
{
	m_CursorPos = { xpos , ypos };

	if (!Statics::ActiveCursor)
		Statics::SceneCamera.MouseLook({ xpos, ypos });
}

void Scene_Assessment2::CursorClickEvent(int button, int action, int mods)
{
}

void Scene_Assessment2::Draw()
{
	if (m_GeoStar)
		m_GeoStar->Draw();

	if (m_NormalsSphere)
		m_NormalsSphere->Draw();

	if (m_ExplodingObject)
		m_ExplodingObject->Draw();
}
