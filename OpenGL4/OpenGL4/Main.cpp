#include "GameObject.h"

GLFWwindow* RenderWindow = nullptr;
glm::ivec2 WindowSize { 800,800 };
float DeltaTime = 0.0f, LastFrame = 0.0f;
GameObject* TestObject = nullptr; 
Camera* SceneCamera = nullptr;
Mesh* SphereMesh = nullptr;

void InitGL();
void InitGLFW();

void Start();
void Update();
void Render();

void CalculateDeltaTime();

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

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glEnable(GL_CULL_FACE);
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

	RenderWindow = glfwCreateWindow(800, 800, "Title", NULL, NULL);

	glfwMakeContextCurrent(RenderWindow);
}

void Start()
{
	SphereMesh = new Mesh(SHAPE::SPHERE, GL_CCW);
	SceneCamera = new Camera(WindowSize, { 0,0,-1 });
	TestObject = new GameObject(*SceneCamera, { 0,0,0 });
	TestObject->SetShader("", "");
	TestObject->SetMesh(SphereMesh);
}

void Update()
{
	while (glfwWindowShouldClose(RenderWindow) == false)
	{
		CalculateDeltaTime();
		glfwPollEvents();
		Render();
	}
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (TestObject)
		TestObject->Draw();

	glfwSwapBuffers(RenderWindow);
}

void CalculateDeltaTime()
{
	float currentFrame = (float)glfwGetTime();
	DeltaTime = currentFrame - LastFrame;
	LastFrame = currentFrame;
}

int Cleanup()
{
	if (TestObject)
		delete TestObject;
	TestObject = nullptr;

	glfwDestroyWindow(RenderWindow);
	glfwTerminate();

	return 0;
}
