// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Mesh.h 
// Description : Mesh Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Helper.h"

enum class SHAPE
{
	UNASSIGNED,
	POINT,
	CUBE,
	PYRAMID,
	SPHERE,
	HEMISPHERE,
	PATCH_TRIANGLE,
	PATCH_TRIANGLE_QUAD,
	PATCH_QUAD,
	PLANE,
	QUAD,
	TRIANGLE
};

struct aiNode;
struct aiScene;
struct aiMesh;
struct aiMaterial;
enum aiTextureType;
class Mesh
{
public:
	/// <summary>
	/// Contruct a mesh with the given shape
	/// </summary>
	/// <param name="_shape"></param>
	Mesh(SHAPE _shape, GLenum _windingOrder = GL_CCW);
	/// <summary>
	/// Construct a 2D Mesh with the given number of sides
	/// </summary>
	/// <param name="_numberOfSides"></param>
	Mesh(unsigned int _numberOfSides, GLenum _windingOrder = GL_CCW);

	/// <summary>
	/// Construct a mesh with a models filepath
	/// </summary>
	/// <param name="_objModel"></param>
	Mesh(std::string _objModel);

	/// <summary>
	/// Construct a mesh with the specified vertices, indices and textures
	/// </summary>
	/// <param name="_vertices"></param>
	/// <param name="_indices"></param>
	/// <param name="_textures"></param>
	Mesh(std::vector<Vertex> _vertices, std::vector<unsigned> _indices, std::vector<Texture> _textures);

	/// <summary>
	/// Mesh Destructor
	/// </summary>
	~Mesh();
	/// <summary>
	/// Draws The Mesh
	/// </summary>
	void Draw();

	/// <summary>
	/// Returns a refrence too the vertices
	/// </summary>
	/// <returns></returns>
	std::vector<Vertex>& GetVertices();

	/// <summary>
	/// Returns a refrence too the indices
	/// </summary>
	/// <returns></returns>
	std::vector<unsigned>& GetIndices();

	/// <summary>
	/// Checks raycast intersection and returns bool
	/// </summary>
	/// <param name="_ray"></param>
	/// <returns></returns>
	bool RayIntersection(Ray _ray, Transform _transform);

	/// <summary>
	/// Checks raycast intersection and returns bool 
	/// along with the point of intersection
	/// </summary>
	/// <param name="_ray"></param>
	/// <param name="_point"></param>
	/// <returns></returns>
	bool RayIntersection(Ray _ray, Transform _transform, glm::vec3& _point);

private:
	/// <summary>
	/// Populates the vertices vector with values required for the specified shape.
	/// </summary>
	/// <param name="_shape"></param>
	void CreateShapeVertices(SHAPE _shape);
	/// <summary>
	/// Populates the indices vector with values required for the specified shape.
	/// </summary>
	/// <param name="_shape"></param>
	void CreateShapeIndices(SHAPE _shape);
	/// <summary>
	/// Populates the vertices vector with values required for the specified 2D shape.
	/// </summary>
	/// <param name="_numberOfSides"></param>
	void CreatePolygonVertices(unsigned int _numberOfSides);
	/// <summary>
	/// Populates the indices vector with values required for the specified 2 shape.
	/// </summary>
	/// <param name="_numberOfSides"></param>
	void CreatePolygonIndices(unsigned int _numberOfSides);
	/// <summary>
	/// Creates the vertexArray, vertex buffer and index buffer, 
	/// populating them with the vertices and indices.
	/// Indices can be disabled by setting _ebo to false
	/// </summary>
	void CreateAndInitializeBuffersNONDSA(bool _ebo = true);
	/// <summary>
	/// Creates the vertexArray, vertex buffer and index buffer, 
	/// populating them with the vertices and indices values.
	/// Indices can be disabled by setting _ebo to false
	/// </summary>
	void CreateAndInitializeBuffersDSA(bool _ebo = true);
	/// <summary>
	/// Converts the given positional value to texture coordinate space (0-1)
	/// </summary>
	/// <param name="_position"></param>
	/// <returns></returns>
	float ToTexCoord(float& _position);
	/// <summary>
	/// Populates Vertices Array With Information For A Sphere Of Specified Fidelity
	/// </summary>
	/// <param name="_fidelity"></param>
	void GenerateSphereVertices(int _fidelity);
	/// <summary>
	/// Populates Indices Array With Information For A Sphere Of Specified Fidelity
	/// </summary>
	/// <param name="_fidelity"></param>
	void GenerateSphereIndices(int _fidelity);
	/// <summary>
	/// Populates Vertices Array With Information For A HemiSphere Of Specified Fidelity
	/// </summary>
	/// <param name="_fidelity"></param>
	void GenerateHemiSphereVertices(int _fidelity);

	/// <summary>
	/// Load the specified model with TinyOBJ loader
	/// </summary>
	/// <param name="_path"></param>
	void LoadModelTinyOBJ(std::string _path);

	/// <summary>
	/// Load the specified model with ASSIMP loader
	/// </summary>
	/// <param name="_path"></param>
	void LoadModelASSIMP(std::string _path);

	/// <summary>
	/// Process the specified ASSIMP node
	/// </summary>
	/// <param name="_node"></param>
	/// <param name="_scene"></param>
	void ProcessNode(aiNode* _node, const aiScene* _scene);

	/// <summary>
	/// Process the specified ASSIMP mesh
	/// </summary>
	/// <param name="_mesh"></param>
	/// <param name="_scene"></param>
	/// <returns></returns>
	Mesh* ProcessMesh(aiMesh* _mesh, const aiScene* _scene);

	/// <summary>
	/// Load all the embedded textures of the ASSIMP model
	/// </summary>
	/// <param name="_mat"></param>
	/// <param name="_type"></param>
	/// <returns></returns>
	std::vector<Texture> LoadTextures(aiMaterial* _mat, aiTextureType _type);

	std::vector<Texture> m_AssimpTextures{};
	std::vector<Mesh*> m_AssimpMeshes{};
	std::vector<unsigned int> m_Indices{};
	std::vector<Vertex> m_Vertices{};

	SHAPE m_Shape{ 0 };

	GLuint m_VertexArrayID{ 0 };
	GLuint m_VertexBufferID{ 0 };
	GLuint m_IndexBufferID{ 0 };

	GLenum m_WindingOrder{GL_CCW};
};

