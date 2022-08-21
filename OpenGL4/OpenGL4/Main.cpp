#include "Helper.h"
#include "Statics.h"
#include "SceneManager.h"

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
		if (key.first == GLFW_KEY_ESCAPE)
			glfwSetWindowShouldClose(Statics::RenderWindow, true);
	}

	SceneManager::KeyEvents();
}

static void CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	SceneManager::MouseEvents(xpos, ypos);
}

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

	glfwSetInputMode(Statics::RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Start()
{
	SceneManager::LoadScene(SCENES::ASSESSMENT1);
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	SceneManager::Draw();

	glfwSwapBuffers(Statics::RenderWindow);
}

int Cleanup()
{
	SceneManager::CleanupScene();
	Statics::Cleanup();

	return 0;
}
