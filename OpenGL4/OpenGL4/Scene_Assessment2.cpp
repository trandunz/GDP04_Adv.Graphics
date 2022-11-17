// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Scene_Assessment2.cpp 
// Description : Scene_Assessment2 Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Scene_Assessment2.h"
#include "TextureLoader.h"
#include "FrameBuffer.h"
#include "ShadowMap.h"
#include "LightManager.h"

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
	if (m_TesselationTriangle)
	{
		delete m_TesselationTriangle;
		m_TesselationTriangle = nullptr;
	}
	if (m_ShadowCube)
	{
		delete m_ShadowCube;
		m_ShadowCube = nullptr;
	}
	if (m_ShadowPlane)
	{
		delete m_ShadowPlane;
		m_ShadowPlane = nullptr;
	}
	if (m_ShadowSphere)
	{
		delete m_ShadowSphere;
		m_ShadowSphere = nullptr;
	}
	if (m_PugModel)
	{
		delete m_PugModel;
		m_PugModel = nullptr;
	}
	if (m_PugObject)
	{
		delete m_PugObject;
		m_PugObject = nullptr;
	}
}

void Scene_Assessment2::Start()
{
	// create point light at 0,20,0
	LightManager::GetInstance().CreatePointLight({ { 0,20,0 } });

	// Create cross and pug models
	m_AsymmetricModel = new Mesh("LowPoly/Cross.obj");
	m_PugModel = new Mesh("Pug/Pug.obj");

	// create pug object
	m_PugObject = new GameObject;
	m_PugObject->SetMesh(m_PugModel);
	m_PugObject->SetShader("Normals3D_Shadows.vert", "BlinnFong3D_Shadows.frag");
	m_PugObject->SetTranslation({ 2,-1,0.0f });
	m_PugObject->SetActiveTextures({ TextureLoader::LoadTexture("Pug.png") });

	// create geostar object
	m_GeoStar = new GameObject();
	m_GeoStar->SetMesh(StaticMesh::Point);
	m_GeoStar->SetShader("PositionOnly.vert", "PointToStar.geo", "BlinnFong3D_Shadows.frag");
	m_GeoStar->SetTranslation({ 0,0,0.0f });
	m_GeoStar->SetRotation({ 1,0,0 }, 90);
	m_GeoStar->SetActiveTextures({ TextureLoader::LoadTexture("Snow.jpg") });

	// create sphere with normals showing object
	m_NormalsSphere = new GameObject();
	m_NormalsSphere->SetMesh(StaticMesh::Sphere);
	m_NormalsSphere->SetShader("Normals3D_Shadows.vert", "BlinnFong3D_Shadows.frag");
	m_NormalsSphere->SetTranslation({ 0,0,-2.0f });
	m_NormalsSphere->SetShowNormals(true);

	// create exploding object
	m_ExplodingObject = new GameObject();
	m_ExplodingObject->SetMesh(m_AsymmetricModel);
	m_ExplodingObject->SetShader("Normals3D_Shadows.vert", "Explode.geo", "BlinnFong3D_Shadows.frag");
	m_ExplodingObject->SetTranslation({ 3,0,-2.0f });
	m_ExplodingObject->SetActiveTextures({ TextureLoader::LoadTexture("LowPoly/Cross.png") });
	m_ExplodingObject->SetScale({ 0.01f, 0.01f, 0.01f });

	// create tesselated triangle / terrain object
	m_TesselationTriangle = new GameObject();
	m_TesselationTriangle->SetMesh(StaticMesh::Patch_Triangle_Quad);
	m_TesselationTriangle->SetShader("PositionPassthrough.vert", "", "TrianglePatch_LOD.tc", "HeightMap_Shadows.te", "BlinnFong3D_Shadows.frag");
	m_TesselationTriangle->SetTranslation({1,-5.0f,-5.0f});
	m_TesselationTriangle->SetRotation({0.5f,0,0}, 90);
	m_TesselationTriangle->SetActiveTextures(
		{ 
			TextureLoader::LoadTexture("Moss.jpg"),
			TextureLoader::LoadTexture("Heightmaps/MossNoise.jpg"),
			TextureLoader::LoadTexture("Heightmaps/MossNoise_Normals.bmp")
		});
	m_TesselationTriangle->Scale({ 5.0f, 5.0f, 5.0f });

	// create cube object
	m_ShadowCube = new GameObject();
	m_ShadowCube->SetMesh(StaticMesh::Cube);
	m_ShadowCube->SetShader("Normals3D_Shadows.vert", "BlinnFong3D_Shadows.frag");
	m_ShadowCube->SetTranslation({ 0.0f,0.5f,2.0f });

	// create plane object
	m_ShadowPlane = new GameObject();
	m_ShadowPlane->SetMesh(StaticMesh::Plane);
	m_ShadowPlane->SetShader("Normals3D_Shadows.vert", "BlinnFong3D_Shadows.frag");
	m_ShadowPlane->SetTranslation({ 0.0f,-10.0f,-2.0f });
	m_ShadowPlane->SetScale({ 30.0f, 1.0f, 30.0f });
	m_ShadowPlane->SetActiveTextures({ TextureLoader::LoadTexture("Moss.jpg")});

	// create sphere object
	m_ShadowSphere = new GameObject();
	m_ShadowSphere->SetMesh(StaticMesh::Sphere);
	m_ShadowSphere->SetShader("Normals3D_Shadows.vert", "BlinnFong3D_Shadows.frag");
	m_ShadowSphere->SetTranslation({ 0.0f,-1.0f,2.0f });
}

void Scene_Assessment2::Update()
{
	m_ElapsedTime += Statics::DeltaTime;
	// If mouse is not active, move camera else move the cube
	if (!Statics::ActiveCursor)
	{
		Statics::ActiveCamera->Movement_Capture();
		Statics::ActiveCamera->Movement();
	}
	else
	{
		if (m_ShadowCube)
		{
			m_ShadowCube->Movement_WASDEQ();
			m_ShadowCube->Update();
		}

	}
	// rotate the pug cos why not
	if (m_PugObject)
	{
		m_PugObject->Rotate({ 0,1,0 }, Statics::DeltaTime * 10.0f);
	}
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
		Statics::ActiveCamera->MouseLook({ xpos, ypos });
}

void Scene_Assessment2::CursorClickEvent(int button, int action, int mods)
{
}

void Scene_Assessment2::Draw()
{
	//
	// Shadow Map Pass
	//
	ShadowMap::GetInstance().Bind();

	if (m_GeoStar)
		m_GeoStar->DrawShadows();
	
	if (m_NormalsSphere)
		m_NormalsSphere->DrawShadows();
	
	if (m_ExplodingObject)
		m_ExplodingObject->DrawShadows();
	
	if (m_TesselationTriangle)
		m_TesselationTriangle->DrawShadows();

	if (m_ShadowCube)
		m_ShadowCube->DrawShadows();

	if (m_ShadowPlane)
		m_ShadowPlane->DrawShadows();

	if (m_ShadowSphere)
		m_ShadowSphere->DrawShadows();

	if (m_PugObject)
		m_PugObject->DrawShadows();

	ShadowMap::GetInstance().Unbind();

	//
	// Color Pass
	//

	FrameBuffer::GetInstance().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (m_GeoStar)
		m_GeoStar->Draw();

	if (m_NormalsSphere)
		m_NormalsSphere->Draw();

	if (m_ExplodingObject)
		m_ExplodingObject->Draw();

	if (m_TesselationTriangle)
		m_TesselationTriangle->Draw();

	if (m_ShadowCube)
		m_ShadowCube->Draw();

	if (m_ShadowPlane)
		m_ShadowPlane->Draw();

	if (m_ShadowSphere)
		m_ShadowSphere->Draw();

	if (m_PugObject)
		m_PugObject->Draw();

	FrameBuffer::GetInstance().Unbind();
	glfwSwapBuffers(Statics::RenderWindow);
}
