// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : FrameBuffer.h 
// Description : FrameBuffer Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "Statics.h"
class FrameBuffer
{
public:
	/// <summary>
	/// Frame buffer singlton constructor 
	/// </summary>
	/// <returns></returns>
	static FrameBuffer& GetInstance()
	{
		static FrameBuffer frameBuffer{};
		return frameBuffer;
	}

	/// <summary>
	/// Binds the frame buffer
	/// </summary>
	void Bind();
	/// <summary>
	/// Unbinds and draws the framebuffer
	/// </summary>
	void Unbind();

	/// <summary>
	/// Seets the shader of tthe frame buffer
	/// </summary>
	/// <param name="_location"></param>
	void SetShader(ShaderProgramLocation _location);

	/// <summary>
	/// Enabled rain effect
	/// </summary>
	void EnableRain();
	/// <summary>
	/// Enables chromatic aberation effect
	/// </summary>
	void EnableCA();
	/// <summary>
	/// Enables CRT effect
	/// </summary>
	void EnableCRT();
	/// <summary>
	/// Disables all effects
	/// </summary>
	void DisableAllEffects();

	Texture GetPositionTexture();
	Texture GetNormalsTexture();
	Texture GetAlbedoTexture();
	Texture GetDepthTexture();
private:
	FrameBuffer();
	~FrameBuffer();

	Transform m_Transform{};
	Texture m_RenderTexture{};
	Texture m_RainTexture{};
	GLuint m_RenderBufferID{};
	GLuint m_FrameBufferID{};
	GLuint m_RainShaderID{};
	GLuint m_CAShaderID{};
	GLuint m_CRTShaderID{};
	GLuint m_ShaderID{};

	Texture m_PositionTexture{};
	Texture m_NormalTexture{};
	Texture m_AlbedoTexture{};
	Texture m_DepthTexture{};

	bool m_EnableRain{ false };
	bool m_EnableCA { false };
	bool m_EnableCRT{ false };
};

