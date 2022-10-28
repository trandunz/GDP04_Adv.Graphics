#include "Scene_Assignment3.h"
#include "TextureLoader.h"
#include "FrameBuffer.h"

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

	m_AnimatedModel = new SkinnedMesh("Resources/Models/Dude/Dude.dae");
	m_DanceAnim = new Animation("Resources/Models/Dude/Dude.dae", m_AnimatedModel);
	m_Animator = new Animator(m_DanceAnim);
	
	m_AssimpObject = new GameObject({ 0,-10,-50 });
	m_AssimpObject->SetSkinnedMesh(m_AnimatedModel);
	//m_AssimpObject->SetAnimator(m_Animator);
	m_AssimpObject->SetShader("SkeletalAnimation.vert", "SingleTexture.frag");
	m_AssimpObject->SetActiveTextures({ TextureLoader::LoadTexture("Dude.png") });
	m_AssimpObject->SetRotation({ 1,0,0 }, -90);
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
	FrameBuffer::GetInstance().Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Skybox::GetInstance().Draw();

	if (m_AssimpObject)
		m_AssimpObject->Draw();

	FrameBuffer::GetInstance().Unbind();
	glfwSwapBuffers(Statics::RenderWindow);
}
