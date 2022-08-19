#pragma once
#include "Helper.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "LightManager.h"
#include "TextureLoader.h"

class Terrain
{
public:
	Terrain(Camera& _camera, std::string _heightMap);
	Terrain(Camera& _camera);
	~Terrain();

	void Draw();

	void SetLightManager(LightManager& _lightManager);
	void SetTexture(Texture _texture);

	void SetTranslation(glm::vec3 _newPosition);
	void Translate(glm::vec3 _translation);

	void SetRotation(glm::vec3 _axis, float _degrees);
	void Rotate(glm::vec3 _axis, float _degrees);

	void SetScale(glm::vec3 _newScale);
	void Scale(glm::vec3 _scaleFactor);

	/// <summary>
	/// Sets the active textures to specified textures.
	/// </summary>
	/// <param name="_textures"></param>
	void SetActiveTextures(std::vector<Texture> _textures);

private:
	void CreateAndInitializeBuffers();

	void GenerateVertices();
	void GenerateIndices();

	void LoadRAWHeightmap(std::string _fileName);
	void LoadHeightmap(std::string _fileName);
	void GenerateRandomHeightmap();
	void Smooth();
	float Average(unsigned _i, unsigned _j);
	bool InBounds(unsigned _i, unsigned _j);

	void SetBlinnFong3DUniforms();

	std::vector<float> m_HeightMap{};
	std::vector<unsigned int> m_Indices{};
	std::vector<Vertex> m_Vertices{};
	std::vector<Texture> m_ActiveTextures{};

	Transform m_Transform{};
	Camera* m_ActiveCamera = nullptr;
	LightManager* m_LightManager{ nullptr };
	GLuint m_ShaderID{ 0 };
	Texture m_Texture{};
	GLuint m_VertexArrayID{ 0 };
	GLuint m_VertexBufferID{ 0 };
	GLuint m_IndexBufferID{ 0 };
};

