#pragma once
#include "Statics.h"
class gBuffer
{
public:
	/// <summary>
	/// Frame buffer singlton constructor 
	/// </summary>
	/// <returns></returns>
	static gBuffer& GetInstance()
	{
		static gBuffer gBuffer{};
		return gBuffer;
	}

	/// <summary>
	/// Binds the frame buffer
	/// </summary>
	void Bind();
	/// <summary>
	/// Unbinds and draws the framebuffer
	/// </summary>
	void Unbind();

	GLuint GetFBO();
	Texture GetPositionTexture();
	Texture GetNormalsTexture();
	Texture GetAlbedoTexture();
	Texture GetDepthTexture();
private:
	gBuffer();
	~gBuffer();

	Transform m_Transform{};

	GLuint m_FrameBufferID{};
	GLuint m_ShaderID{};

	Texture m_PositionTexture{};
	Texture m_NormalTexture{};
	Texture m_AlbedoTexture{};
	Texture m_DepthTexture{};
};

