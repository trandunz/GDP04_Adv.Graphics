#include "gBuffer.h"
#include "TextureLoader.h"

void gBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	GLfloat clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glClearNamedFramebufferfv(m_FrameBufferID, GL_COLOR, 0, clearColor);
	glEnable(GL_DEPTH_TEST);
}

void gBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
}

GLuint gBuffer::GetFBO()
{
	return m_FrameBufferID;
}

Texture gBuffer::GetPositionTexture()
{
	return m_PositionTexture;
}

Texture gBuffer::GetNormalsTexture()
{
	return m_NormalTexture;
}

Texture gBuffer::GetAlbedoTexture()
{
	return m_AlbedoTexture;
}

Texture gBuffer::GetDepthTexture()
{
	return m_DepthTexture;
}

gBuffer::gBuffer()
{
	m_PositionTexture = TextureLoader::CreatePositionTexture();
	m_NormalTexture = TextureLoader::CreatePositionTexture();
	m_AlbedoTexture = TextureLoader::CreateAlbedoTexture();
	m_DepthTexture = TextureLoader::CreateDepthTexture();

	// Create frame buffers and render buffers
	glCreateFramebuffers(1, &m_FrameBufferID);

	// Set frame buffer attachment
	glNamedFramebufferTexture(m_FrameBufferID, GL_COLOR_ATTACHMENT0, m_PositionTexture.ID, 0);
	glNamedFramebufferTexture(m_FrameBufferID, GL_COLOR_ATTACHMENT1, m_NormalTexture.ID, 0);
	glNamedFramebufferTexture(m_FrameBufferID, GL_COLOR_ATTACHMENT2, m_AlbedoTexture.ID, 0);
	glNamedFramebufferTexture(m_FrameBufferID, GL_DEPTH_ATTACHMENT, m_DepthTexture.ID, 0);

	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0 , GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glNamedFramebufferDrawBuffers(m_FrameBufferID, 3, attachments);

	auto status = glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
		Print("Framebuffer Error: " + std::to_string(status));

	glBindTexture(GL_TEXTURE_2D, 0);
}

gBuffer::~gBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDeleteFramebuffers(1, &m_FrameBufferID);
}
