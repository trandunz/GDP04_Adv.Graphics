// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Terrain.h 
// Description : Terrain Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Helper.h"
#include "ShaderLoader.h"
#include "Camera.h"
#include "LightManager.h"
#include "TextureLoader.h"

class Terrain
{
public:
	/// <summary>
	/// Construct a terrain with the specified heightmap
	/// </summary>
	/// <param name="_heightMap"></param>
	/// <param name="_fileExtension"></param>
	Terrain(std::string _heightMap, std::string _fileExtension);
	/// <summary>
	/// Create a terrain with a random heightmap
	/// </summary>
	Terrain();
	/// <summary>
	/// Terrain Destructor
	/// </summary>
	~Terrain();
	
	/// <summary>
	/// Draws the terrain to the frame buffer
	/// </summary>
	void Draw();

	/// <summary>
	/// Sets the firs texture of the terrain
	/// </summary>
	/// <param name="_texture"></param>
	void SetTexture(Texture _texture);

	/// <summary>
	/// Sets the translation of the terrain
	/// </summary>
	/// <param name="_newPosition"></param>
	void SetTranslation(glm::vec3 _newPosition);
	/// <summary>
	/// Translates the terrain by given amount
	/// </summary>
	/// <param name="_translation"></param>
	void Translate(glm::vec3 _translation);

	/// <summary>
	/// Sets the rotation of the terrain
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void SetRotation(glm::vec3 _axis, float _degrees);
	/// <summary>
	/// Rotate the terrain by a given amount
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void Rotate(glm::vec3 _axis, float _degrees);

	/// <summary>
	/// Set the scale of the terrain
	/// </summary>
	/// <param name="_newScale"></param>
	void SetScale(glm::vec3 _newScale);
	/// <summary>
	/// Scale the terrain by some amount
	/// </summary>
	/// <param name="_scaleFactor"></param>
	void Scale(glm::vec3 _scaleFactor);

	/// <summary>
	/// Sets the active textures to specified textures.
	/// </summary>
	/// <param name="_textures"></param>
	void SetActiveTextures(std::vector<Texture> _textures);

	/// <summary>
	/// Returns a refrence to the transform
	/// </summary>
	/// <returns></returns>
	Transform& GetTransform() { return m_Transform; }

private:

	/// <summary>
	/// Creates the vertexArray, vertex buffer and index buffer, 
	/// populating them with the vertices and indices values.
	/// </summary>
	void CreateAndInitializeBuffers(bool _dsa = true);
	/// <summary>
	/// Populates the vertex vector with vertices
	/// </summary>
	void GenerateVertices();
	/// <summary>
	/// Populates the indices vector with indices
	/// </summary>
	void GenerateIndices();

	/// <summary>
	/// Loads a heightmap file of type RAW into the heightmap vector
	/// </summary>
	/// <param name="_fileName"></param>
	void LoadRAWHeightmap(std::string _fileName);
	/// <summary>
	/// Loads a heightmap file into the heightmap vector from an image
	/// </summary>
	/// <param name="_fileName"></param>
	void LoadImageHeightmap(std::string _fileName);
	/// <summary>
	/// Populates the heightmap vector with randomized perlin noise
	/// </summary>
	void GenerateRandomHeightmap();
	/// <summary>
	/// Smooths the values of the height map vector by taking the average of surrounding heights
	/// </summary>
	void Smooth();
	/// <summary>
	/// Returns the average value of the height map vector at location _i, _j by looking at the surrounding values
	/// </summary>
	/// <param name="_i"></param>
	/// <param name="_j"></param>
	/// <returns></returns>
	float Average(unsigned _i, unsigned _j);
	/// <summary>
	/// Returns bool based on if the locatin _i, _j is in the range of the heightmap vector
	/// </summary>
	/// <param name="_i"></param>
	/// <param name="_j"></param>
	/// <returns></returns>
	bool InBounds(unsigned _i, unsigned _j);
	
	/// <summary>
	/// Sets up blinn phong uniforms for lighting shader
	/// </summary>
	void SetBlinnFong3DUniforms();

	std::vector<float> m_HeightMap{};
	std::vector<unsigned int> m_Indices{};
	std::vector<Vertex> m_Vertices{};
	std::vector<Texture> m_ActiveTextures{};

	Transform m_Transform{};
	GLuint m_ShaderID{ 0 };
	Texture m_Texture{};
	GLuint m_VertexArrayID{ 0 };
	GLuint m_VertexBufferID{ 0 };
	GLuint m_IndexBufferID{ 0 };
};

