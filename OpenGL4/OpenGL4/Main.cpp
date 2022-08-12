#include "GameObject.h"
#include "TextureLoader.h"
#include "Skybox.h"
#include "Terrain.h"

GLFWwindow* RenderWindow = nullptr;
glm::ivec2 WindowSize { 800,800 };
float DeltaTime = 0.0f, LastFrame = 0.0f;
GameObject* TestObject = nullptr; 
Camera* SceneCamera = nullptr;
Mesh* SphereMesh = nullptr;
Mesh* CubeMesh = nullptr;
Mesh* StatueMesh = nullptr;
Terrain* TerrainMesh = nullptr;
LightManager* lightManager = nullptr;

KEYMAP Keymap{};
Skybox* skyboxRef = nullptr;

static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		Keymap[key] = true;
	else if (action == GLFW_RELEASE)
		Keymap[key] = false;

	for (auto& key : Keymap)
	{
		if (key.second)
		{
			if (key.first == GLFW_KEY_ESCAPE)
			{
				glfwSetWindowShouldClose(RenderWindow, true);
			}
		}
	}
}

static void CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (SceneCamera)
	{
		SceneCamera->MouseLook(DeltaTime, { xpos, ypos });
	}
}

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

	RenderWindow = glfwCreateWindow(800, 800, "Title", NULL, NULL);

	glfwMakeContextCurrent(RenderWindow);

	glfwSetKeyCallback(RenderWindow, KeyCallback);
	glfwSetCursorPosCallback(RenderWindow, CursorCallback);

	glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Start()
{
	//StatueMesh = new Mesh("Angel/Angel.obj");
	SphereMesh = new Mesh(SHAPE::SPHERE, GL_CCW);
	CubeMesh = new Mesh(SHAPE::CUBE, GL_CCW);

	SceneCamera = new Camera(WindowSize, { 0,0,5 });
	TestObject = new GameObject(*SceneCamera, { 0,0,0 });

	lightManager = new LightManager(*SceneCamera);
	DirectionalLight sun{};
	sun.Direction = {-1,-1,0};
	lightManager->CreateDirectionalLight(sun);
	
	TextureLoader::Init({
		"World.jpg",
		"Grass.jpg"
		});

	TestObject->SetActiveTextures({TextureLoader::LoadTexture("World.jpg")});
	TestObject->SetMesh(SphereMesh);
	TestObject->SetShader("Fog.vert", "Fog.frag");
	TestObject->SetLightManager(*lightManager);

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

	TerrainMesh = new Terrain(*SceneCamera);
	TerrainMesh->SetLightManager(*lightManager);
	TerrainMesh->SetTexture(TextureLoader::LoadTexture("Grass.jpg"));
	TerrainMesh->SetScale({ 0.05f,0.05f,0.05f });
	TerrainMesh->SetTranslation({ 0.0f,-20.0f,0.0f });
}

void Update()
{
	while (glfwWindowShouldClose(RenderWindow) == false)
	{
		CalculateDeltaTime();
		glfwPollEvents();

		if (SceneCamera)
		{
			SceneCamera->Movement_Capture(Keymap);
			SceneCamera->Movement(DeltaTime);
		}


		Render();
	}
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	skyboxRef->Draw();

	if (TestObject)
		TestObject->Draw();

	if (TerrainMesh)
		TerrainMesh->Draw();

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
	skyboxRef = nullptr;

	if (lightManager)
		delete lightManager;
	lightManager = nullptr;

	if (TerrainMesh)
		delete TerrainMesh;
	TerrainMesh = nullptr;

	if (StatueMesh)
		delete StatueMesh;
	StatueMesh = nullptr;

	if (CubeMesh)
		delete CubeMesh;
	CubeMesh = nullptr;

	if (SphereMesh)
		delete SphereMesh;
	SphereMesh = nullptr;

	if (SceneCamera)
		delete SceneCamera;
	SceneCamera = nullptr;

	if (TestObject)
		delete TestObject;
	TestObject = nullptr;

	glfwDestroyWindow(RenderWindow);
	glfwTerminate();

	return 0;
}
