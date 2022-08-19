#include "GameObject.h"
#include "TextureLoader.h"
#include "Skybox.h"
#include "Terrain.h"

GLFWwindow* RenderWindow = nullptr;
glm::ivec2 WindowSize { 800,800 };
bool BlackBars = true;
GameObject* ModelObject = nullptr;
Camera* SceneCamera = nullptr;
Mesh* SphereMesh = nullptr;
Mesh* CubeMesh = nullptr;
Mesh* CrossMesh = nullptr;
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

static void CursorCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (SceneCamera)
	{
		SceneCamera->MouseLook(Statics::DeltaTime, { xpos, ypos });
	}
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

	RenderWindow = glfwCreateWindow(800, 800, "Title", NULL, NULL);

	glfwMakeContextCurrent(RenderWindow);

	glfwSetKeyCallback(RenderWindow, KeyCallback);
	glfwSetCursorPosCallback(RenderWindow, CursorCallback);

	glfwSetInputMode(RenderWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Start()
{
	CrossMesh = new Mesh("Cross/Cross.obj");
	SphereMesh = new Mesh(SHAPE::SPHERE, GL_CCW);
	CubeMesh = new Mesh(SHAPE::CUBE, GL_CCW);

	SceneCamera = new Camera(WindowSize, { 0,0,5 });
	ModelObject = new GameObject(*SceneCamera, { 0,0,0 });

	lightManager = new LightManager(*SceneCamera);
	DirectionalLight sun{};
	sun.Direction = {-1,-1,0};
	lightManager->CreateDirectionalLight(sun);
	
	TextureLoader::Init({
		"World.jpg",
		"Grass.jpg"
		});

	ModelObject->SetActiveTextures({TextureLoader::LoadTexture("Cross/Cross.png")});
	ModelObject->SetMesh(CrossMesh);
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
		Statics::CalculateDeltaTime();
		glfwPollEvents();

		if (SceneCamera)
		{
			SceneCamera->Movement_Capture(Keymap);
			SceneCamera->Movement(Statics::DeltaTime);
		}

		if (ModelObject)
		{
			ModelObject->Rotate({ 0,1,0 }, 2 * glm::radians(glfwGetTime()));
		}

		Render();
	}
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if (BlackBars)
	{
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 50, WindowSize.x, WindowSize.y - 100);
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

	glfwSwapBuffers(RenderWindow);
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

	if (CrossMesh)
		delete CrossMesh;
	CrossMesh = nullptr;

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

	glfwDestroyWindow(RenderWindow);
	glfwTerminate();

	return 0;
}
