#include "Scene_Assignment3.h"
#include "TextureLoader.h"
#include "FrameBuffer.h"
#include "gBuffer.h"
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

	LightManager::GetInstance().SetMaxPointLights(100);
	for (int i = 0; i < 10; i++)
	{
		float r = (rand() % 255) / 255.0f;
		float g = (rand() % 255) / 255.0f;
		float b = (rand() % 255) / 255.0f;
		LightManager::GetInstance().CreatePointLight({ {(rand() % 30) - 15,10,(rand() % 30) - 25}, {r, g, b, 0.5f} });
	}
	
	FrameBuffer::GetInstance().SetShader("Framebuffer.vert","BlinnFong3D_Deferred.frag");

	m_AnimatedModel = new SkinnedMesh("Resources/Models/Dude/Dude.dae");
	
	m_AssimpObject = new GameObject({ 0,-10,-10});
	m_AssimpObject->SetSkinnedMesh(m_AnimatedModel);
	//m_AssimpObject->SetMesh(StaticMesh::Cube);
	m_AssimpObject->SetActiveTextures({ TextureLoader::LoadTexture("Dude.png") });
	m_AssimpObject->SetRotation({ 1,0,0 }, -90);

	m_PlaneObject = new GameObject({ 0,-10,0});
	m_PlaneObject->SetActiveTextures({ TextureLoader::LoadTexture("Grass.jpg") });
	m_PlaneObject->SetMesh(StaticMesh::Triangle_Quad);
	m_PlaneObject->SetScale({ 100,100,1 });
	m_PlaneObject->SetRotation({ 1,0,0 }, -90);
}

void Scene_Assignment3::Update()
{
	m_ElapsedTime += Statics::DeltaTime;
	Statics::SceneCamera.Movement_Capture();
	Statics::SceneCamera.Movement();

	if (m_AssimpObject)
		m_AssimpObject->Update();
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
	// gBuffer Pass
	gBuffer::GetInstance().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (m_AssimpObject)
		m_AssimpObject->DrawToGBuffer();

	if (m_PlaneObject)
		m_PlaneObject->DrawToGBuffer();

	gBuffer::GetInstance().Unbind();

	// Lighting Pass
	FrameBuffer::GetInstance().Bind();
	FrameBuffer::GetInstance().Unbind();
	
	// Forward Pass
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer::GetInstance().GetFBO());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitNamedFramebuffer(gBuffer::GetInstance().GetFBO(), 0, 0, 0, Statics::WindowSize.x, Statics::WindowSize.y, 0, 0, Statics::WindowSize.x, Statics::WindowSize.y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	LightManager::GetInstance().Draw();

	glDisable(GL_BLEND);

	// Swap Buffers
	glfwSwapBuffers(Statics::RenderWindow);
}
