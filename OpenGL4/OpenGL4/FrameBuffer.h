#pragma once
#include "Statics.h"
class FrameBuffer
{
public:
	static FrameBuffer& GetInstance()
	{
		static FrameBuffer frameBuffer{};
		return frameBuffer;
	}

	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void Unbind();

	void SetShader(ShaderProgramLocation _location);

	void EnableRain();
	void EnableCA();
	void EnableCRT();
	void DisableAllEffects();
private:
	Transform m_Transform{};
	Texture m_RenderTexture{};
	Texture m_RainTexture{};
	GLuint m_RenderBufferID{};
	GLuint m_FrameBufferID{};
	GLuint m_RainShaderID{};
	GLuint m_CAShaderID{};
	GLuint m_CRTShaderID{};
	GLuint m_ShaderID{};

	bool m_EnableRain{ false };
	bool m_EnableCA { false };
	bool m_EnableCRT{ false };
};

