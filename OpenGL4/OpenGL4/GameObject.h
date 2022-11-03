// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : GameObject.h 
// Description : GameObject Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once

#include "Statics.h"
#include "StaticMesh.h"
#include "SkinnedMesh.h"
#include "Animator.h"

struct Collider;
class GameObject
{
public:
	/// <summary>
	/// GameObject contructor
	/// </summary>
	/// <param name="_position"></param>
	GameObject(glm::vec3 _position = {0,0,0});
	
	/// <summary>
	/// GameObject Destructor
	/// </summary>
	virtual ~GameObject();

	/// <summary>
	/// Handles moving the object with keyboard.
	/// W: Forward
	/// S: Backward
	/// A: Left
	/// D: Right
	/// Q: Down
	/// E: Up
	/// </summary>
	void Movement_WASDEQ();

	/// <summary>
	/// Handles moving the object with keyboard.
	/// Y: Forward
	/// G: Backward
	/// H: Left
	/// J: Right
	/// </summary>
	void Movement_YGHJ();

	/// <summary>
	/// Update function for GameObject.
	/// Should be called every frame.
	/// </summary>
	virtual void Update();

	/// <summary>
	/// Draws The gameobject if it has a mesh attached
	/// </summary>
	void Draw();

	/// <summary>
	/// Draws the gameobject's depth to the shadow map
	/// </summary>
	void DrawShadows();

	/// <summary>
	/// Attaches a mesh to be used for drawing.
	/// </summary>
	/// <param name="_mesh"></param>
	void SetMesh(Mesh* _mesh);

	/// <summary>
	/// Returns the attached mesh.
	/// </summary>
	/// <returns></returns>
	Mesh* GetMesh();

	void SetSkinnedMesh(SkinnedMesh* _skinnedMesh);

	void SetAnimator(Animator* _animator);

	/// <summary>
	/// Returns a copy of the transform
	/// </summary>
	/// <returns></returns>
	Transform GetTransform() const;

	/// <summary>
	/// Sets the position of the gameObject
	/// </summary>
	/// <param name="_newPosition"></param>
	void SetTranslation(glm::vec3 _newPosition);
	/// <summary>
	/// Translates the position of the gameObject by an amount
	/// </summary>
	/// <param name="_translation"></param>
	void Translate(glm::vec3 _translation);

	/// <summary>
	/// Sets the rotation of the gameObject
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void SetRotation(glm::vec3 _axis, float _degrees);
	/// <summary>
	/// Rotates the gameobject by an amount
	/// </summary>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void Rotate(glm::vec3 _axis, float _degrees);

	/// <summary>
	/// Sets the scale of the gameobject
	/// </summary>
	/// <param name="_newScale"></param>
	void SetScale(glm::vec3 _newScale);

	/// <summary>
	/// Scales the gameObject by amount
	/// </summary>
	/// <param name="_scaleFactor"></param>
	void Scale(glm::vec3 _scaleFactor);

	/// <summary>
	/// Force sets the transform / model matrix
	/// </summary>
	/// <param name="_newModel"></param>
	void SetModel(glm::mat4 _newModel);

	/// <summary>
	/// Rotates the gameObject around the position on the given axis by specified degrees.
	/// </summary>
	/// <param name="_position"></param>
	/// <param name="_axis"></param>
	/// <param name="_degrees"></param>
	void RotateAround(glm::vec3&& _position, glm::vec3&& _axis, float&& _degrees);

	/// <summary>
	/// Sets the active textures to specified textures.
	/// </summary>
	/// <param name="_textures"></param>
	void SetActiveTextures(std::vector<Texture> _textures);

	/// <summary>
	/// Returns the vector of active textures
	/// </summary>
	/// <returns></returns>
	std::vector<Texture> GetActiveTextures();

	/// <summary>
	/// Sets the shader program to use for rendering
	/// </summary>
	/// <param name="_newShader"></param>
	void SetShader(std::string _vertexSource, std::string _fragmentSource);

	/// <summary>
	/// Sets the shader program to use for rendering
	/// </summary>
	/// <param name="_newShader"></param>
	void SetShader(std::string _vertexSource, std::string _geoSource, std::string _fragmentSource);

	/// <summary>
	/// Sets the shader program to use for rendering
	/// </summary>
	/// <param name="_newShader"></param>
	void SetShader(std::string _vertexSource, std::string _geoSource, std::string _tcSource, std::string _fragmentSource);

	/// <summary>
	/// Sets the shader program to use for rendering
	/// </summary>
	/// <param name="_newShader"></param>
	void SetShader(std::string _vertexSource, std::string _geoSource, std::string _tcSource, std::string _teSource, std::string _fragmentSource);

	/// <summary>
	/// Returns the current shader program used for rendering
	/// </summary>
	/// <returns></returns>
	GLuint GetShader();

	/// <summary>
	/// Clears the input vector to prevent further movement
	/// </summary>
	void ClearInputVector();

	/// <summary>
	/// Toggle Rim Lighting If Blinn-Phong Shader Enabled
	/// </summary>
	/// <param name="_rimLighting"></param>
	void SetRimLighting(bool _rimLighting);

	/// <summary>
	/// Checks raycast intersection with current mesh and returns bool
	/// </summary>
	/// <param name="_ray"></param>
	/// <returns></returns>
	bool RayIntersection(Ray _ray);

	/// <summary>
	/// Checks raycast intersection with current mesh and returns bool 
	/// along with the point of intersection
	/// </summary>
	/// <param name="_ray"></param>
	/// <param name="_point"></param>
	/// <returns></returns>
	bool RayIntersection(Ray _ray, glm::vec3& _point);

	bool PointIntersection(glm::vec3 _point);

	/// <summary>
	/// Toggles a stencil outline around the object
	/// </summary>
	/// <param name="_outline"></param>
	void SetStencilOutlineActive(bool _outline);

	/// <summary>
	/// Billboards any object too the camera
	/// </summary>
	/// <param name="_object"></param>
	/// <param name="_relativePos"></param>
	/// <param name="_scale"></param>
	void BillboardObjectToCamera(glm::vec3 _relativePos, glm::vec3 _scale);

	/// <summary>
	/// Toggle visible shape normals
	/// </summary>
	/// <param name="_showNormals"></param>
	void SetShowNormals(bool _showNormals);

	void SetCollider(Collider* _collider);
	Collider* GetCollider();

	glm::vec4 GetInput();

protected:

	/// <summary>
	/// Sets Uniforms for Use With Normals3D.vert
	/// </summary>
	void SetNormals3DVertUniforms(GLuint _shaderID);

	/// <summary>
	/// Handles Setting Blinn-Phong Shader Uniforms.
	/// Requres Blinn-Phong Shader Active
	/// </summary>
	void SetBlinnFong3DUniforms();

	/// <summary>
	/// Handles setting Rim Lighting Shader Uniforms (Combine With Blinn-Phong To Work)
	/// Requires Blinn-Phone Shader Active
	/// </summary>
	void SetRimLighingUniforms();

	/// <summary>
	/// Sets Reflection Specific Shader Uniforms
	/// </summary>
	void SetReflectionUniforms();

	/// <summary>
	/// Set Reflection Map Specific Shader Uniforms.
	/// Requires: Blinn-Phong_Reflection or Reflection Map Shaders
	/// </summary>
	void SetReflectionMapUniforms();

	/// <summary>
	/// Set Uniforms For Single Texture
	/// </summary>
	void SetSingleTextureUniforms();

	/// <summary>
	/// Set uniforms for a fog vertex and fragment shaders
	/// </summary>
	void SetFogUniforms();

	/// <summary>
	/// Set uniforms for growing moss frag shader
	/// </summary>
	void SetMossUniforms();

	/// <summary>
	/// Set uniforms for position only vertex shader
	/// </summary>
	void SetPositionOnlyUniforms();

	/// <summary>
	/// Set the uniforms for the explode geometry shader
	/// </summary>
	/// <param name="_shaderID"></param>
	void SetExplodeUniforms(GLuint _shaderID);

	/// <summary>
	/// Set the unfiroms for the single color frag shader
	/// </summary>
	/// <param name="_shaderID"></param>
	/// <param name="_color"></param>
	void SetSingleColorUniforms(GLuint _shaderID, glm::vec3 _color = {1,1,1});

	/// <summary>
	/// Set the uniforms for the triangle patch LOD tc shader
	/// </summary>
	/// <param name="_shaderID"></param>
	void SetTrianglePatchLODUniforms(GLuint _shaderID);

	/// <summary>
	/// Set the uniforms for the heighmap te shader
	/// </summary>
	/// <param name="_shaderID"></param>
	void SetHeightMapUniforms(GLuint _shaderID);

	/// <summary>
	/// Set the uniforms for the blinnPhong with shadows frag shader
	/// </summary>
	void SetBlinnFong3DShadowsUniform();
	
	/// <summary>
	/// Set the uniforms for the shadowmap vert shader
	/// </summary>
	void SetShadowMapUniforms();
	/// <summary>
	/// Set the uniforms for the heightmap shadow map shader
	/// </summary>
	void SetHeightmapShadowsUniforms();

	void SetSkinnedMeshUniforms();


	bool m_ShowNormals = false;
	bool m_RimLighting = false;
	bool m_StencilOutline = Statics::StencilTest;
	std::vector<Texture> m_ActiveTextures{};
	GLuint m_ShaderID{0};
	GLuint m_StencilShaderID{ 0 };
	GLuint m_NormalsShaderID{ 0 };
	GLuint m_ShadowMapShaderID{ 0 };
	ShaderProgramLocation m_ShaderLocation{};
	glm::vec4 m_Input{};
	float m_MovementSpeed = 5.0f;
	Mesh* m_Mesh = nullptr;
	Transform m_Transform{};
	Collider* m_Collider{ nullptr };
	SkinnedMesh* m_SkinnedMesh{ nullptr };
	Animator* m_Animator{ nullptr };
};

