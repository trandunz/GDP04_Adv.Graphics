// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : ShadowMap.h 
// Description : ShadowMap Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Statics.h"
class ShadowMap
{
public:
	/// <summary>
	/// ShadowMap singleton constructor 
	/// </summary>
	/// <returns></returns>
	static ShadowMap& GetInstance()
	{
		static ShadowMap shadowMap;
		return shadowMap;
	}

	/// <summary>
	/// Bind the shadow map
	/// </summary>
	void Bind();
	/// <summary>
	/// Unbind the shadow map
	/// </summary>
	void Unbind();

	/// <summary>
	/// Return the lightVP matrix of the first spotlight
	/// </summary>
	/// <returns></returns>
	glm::mat4 GetLightVPMatrix();
	/// <summary>
	/// Return the shadow map depth texture
	/// </summary>
	/// <returns></returns>
	Texture GetShadowMapTexture();

private:
	ShadowMap();
	~ShadowMap();

	Texture m_DepthTexture{};
	GLuint m_FrameBufferID{};
};

