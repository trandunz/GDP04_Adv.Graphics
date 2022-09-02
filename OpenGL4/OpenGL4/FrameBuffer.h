#pragma once
#include "StaticMesh.h"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Bind();
	void Unbind();

	void SetShader(ShaderProgramLocation _location);

private:
	Transform m_Transform{};
	Texture m_RenderTexture{};
	GLuint m_RenderBufferID{};
	GLuint m_FrameBufferID{};
	GLuint m_ShaderID{};
};

