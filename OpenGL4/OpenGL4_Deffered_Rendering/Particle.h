// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Particle.h 
// Description : Particle Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Statics.h"

class Particle
{
public:
	/// <summary>
	/// Particle constructor
	/// </summary>
	/// <param name="_pos"></param>
	/// <param name="_velocity"></param>
	/// <param name="_lifeTime"></param>
	Particle(glm::vec3 _pos, glm::vec3 _velocity, float _lifeTime);
	/// <summary>
	/// Particle Destructor
	/// </summary>
	~Particle();

	/// <summary>
	/// Particle update
	/// </summary>
	void Update();
	/// <summary>
	/// Draw the particle
	/// </summary>
	void Draw();

	/// <summary>
	/// Set tthe shader of the particle
	/// </summary>
	/// <param name="_vert"></param>
	/// <param name="_geo"></param>
	/// <param name="_frag"></param>
	void SetShader(std::string _vert, std::string _geo, std::string _frag);
	/// <summary>
	/// Set the shader of the particle
	/// </summary>
	/// <param name="_shader"></param>
	void SetShader(GLuint _shader);

private:
	/// <summary>
	/// Reset the particles values back to the ones specified on construction
	/// </summary>
	void ResetToInitialValues();

	Transform m_Transform{};
	glm::vec3 m_Velocity{};
	float m_LifeTime{};

	glm::vec3 m_StartPos{};
	glm::vec3 m_StartVelocity{};
	float m_ElapsedTime{};

	GLuint m_ShaderID{};
};

