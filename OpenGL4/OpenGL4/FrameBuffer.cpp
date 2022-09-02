#include "FrameBuffer.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "Statics.h"

FrameBuffer::FrameBuffer()
{
	m_Transform.scale = { 2,2,2 };
	UpdateModelValueOfTransform(m_Transform);

	m_ShaderID = ShaderLoader::CreateShader("Framebuffer.vert", "Framebuffer.frag");

	glGenFramebuffers(1, &m_FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

	m_RenderTexture = TextureLoader::CreateRenderTexture();

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTexture.ID, 0);

	glGenRenderbuffers(1, &m_RenderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Statics::WindowSize.x, Statics::WindowSize.y);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);
	auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
		Print("Framebuffer Error: " + std::to_string(status)); // Its throwing the error here!!

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

FrameBuffer::~FrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glUseProgram(m_ShaderID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_RenderTexture.ID);
	ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "Model", m_Transform.transform);

	StaticMesh::Quad->Draw();

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::SetShader(ShaderProgramLocation _location)
{
	m_ShaderID = ShaderLoader::CreateShader(_location.vertShader, _location.fragShader);
}
