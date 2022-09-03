// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Main.cpp 
// Description : Main Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Statics.h"
#include "SceneManager.h"
#include "FrameBuffer.h"

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		Statics::Keymap[key] = true;
	}
	else if (action == GLFW_RELEASE)
		Statics::Keymap[key] = false;

	for (auto& key : Statics::Keymap)
	{
		if (key.second == true)
		{
			// Escape is pressed
			if (key.first == GLFW_KEY_ESCAPE)
			{
				glfwSetWindowShouldClose(Statics::RenderWindow, true);

				// Only on press once please
				key.second = false;
			}
			// Tab is pressed
			if (key.first == GLFW_KEY_TAB)
			{
				// Only on press once please
				key.second = false;

				// Toggle mouse cursor
				Statics::ActiveCursor = !Statics::ActiveCursor;
				if (Statics::ActiveCursor)
					glfwSetInputMode(Statics::RenderWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				else
					glfwSetInputMode(Statics::RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	}

	SceneManager::KeyEvents();
}

static void CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	SceneManager::CursorMoveEvent(xpos, ypos);
}

static void CursorClickCallback(GLFWwindow* window, int button, int action, int mods)
{
	SceneManager::CursorClickEvent(button, action, mods);
}

FrameBuffer* m_FrameBuffer = nullptr;

void InitGL();
void InitGLFW();

void Start();
void Update();
void Render();

int Cleanup();

int main()
{
	InitGLFW();
	InitGL();

	Start();
	Update();

	return Cleanup();
}

void InitGL()
{
	if (glewInit() != GLEW_OK)
		exit(0);

	//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void InitGLFW()
{
	if (glfwInit() != GLFW_TRUE)
		exit(0);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Statics::RenderWindow = glfwCreateWindow(800, 800, "Title", NULL, NULL);

	glfwMakeContextCurrent(Statics::RenderWindow);

	glfwSetKeyCallback(Statics::RenderWindow, KeyCallback);
	glfwSetCursorPosCallback(Statics::RenderWindow, CursorCallback);
	glfwSetMouseButtonCallback(Statics::RenderWindow, CursorClickCallback);

	if (!Statics::ActiveCursor)
		glfwSetInputMode(Statics::RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Start()
{
	Statics::UpdateWindowSize();
	StaticMesh::Init();
	SceneManager::LoadScene(SCENES::ASSESSMENT1);
	m_FrameBuffer = new FrameBuffer;
}

void Update()
{
	while (glfwWindowShouldClose(Statics::RenderWindow) == false)
	{
		Statics::CalculateDeltaTime();
		Statics::UpdateWindowSize();
		glfwPollEvents();

		SceneManager::Update();

		Render();

		SceneManager::ChangeToPendingScene();
	}
}

void Render()
{
	if (m_FrameBuffer)
		m_FrameBuffer->Bind();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (Statics::BlackBars)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 50, Statics::WindowSize.x, Statics::WindowSize.y - 100);
	}

	SceneManager::Draw();

	if (Statics::BlackBars)
		glDisable(GL_SCISSOR_TEST);

	if (m_FrameBuffer)
		m_FrameBuffer->Unbind();

	glfwSwapBuffers(Statics::RenderWindow);
}

int Cleanup()
{
	SceneManager::CleanupScene();
	if (m_FrameBuffer)
		delete m_FrameBuffer;
	m_FrameBuffer = nullptr;
	StaticMesh::Cleanup();
	Statics::Cleanup();

	return 0;
}
