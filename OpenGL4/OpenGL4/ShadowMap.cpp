#include "ShadowMap.h"
#include "StaticMesh.h"
#include "ShaderLoader.h"
#include "LightManager.h"

ShadowMap::ShadowMap()
{
	m_Transform.scale = { 2,2,2 };
	UpdateModelValueOfTransform(m_Transform);

	m_ShaderID = ShaderLoader::CreateShader("ShadowMap.vert", "ShadowMap.frag");

	if (Statics::DSA)
	{
		glGenTextures(1, &m_DepthTexture.ID);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture.ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Statics::WindowSize.x, Statics::WindowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glCreateFramebuffers(1, &m_FrameBufferID);
		glNamedFramebufferTexture(m_FrameBufferID, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture.ID);

		auto status = glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			Print("Framebuffer Error: " + std::to_string(status));

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		glGenTextures(1, &m_DepthTexture.ID);
		glBindTexture(GL_TEXTURE_2D, m_DepthTexture.ID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Statics::WindowSize.x, Statics::WindowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glGenFramebuffers(1, &m_FrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture.ID, 0);

		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			Print("Framebuffer Error: " + std::to_string(status));

		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

ShadowMap::~ShadowMap()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDeleteFramebuffers(1, &m_FrameBufferID);
	glDeleteTextures(1, &m_DepthTexture.ID);
}

void ShadowMap::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	if (Statics::DSA)
	{
		GLfloat clearColor[] = { 0.0f,0.0f,0.0f, 1.0f };
		glClearNamedFramebufferfv(m_FrameBufferID, GL_DEPTH, 0, clearColor);
	}
	else
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}
	glEnable(GL_DEPTH_TEST);
}

void ShadowMap::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glUseProgram(m_ShaderID);
	
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "LightVPMatrix", GetLightVPMatrix());
	ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "Model", m_Transform.transform);

	StaticMesh::Quad->Draw();

	glUseProgram(0);
}

void ShadowMap::SetShader(ShaderProgramLocation _location)
{
	m_ShaderID = ShaderLoader::CreateShader(_location.vertShader, _location.fragShader);
}

glm::mat4 ShadowMap::GetLightVPMatrix()
{
	glm::mat4 lightViewMatrix =	glm::lookAt
	(
		glm::vec3{0.0f, 10.0f, 0.0f},
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,1.0f }
	);

	glm::mat4 lightVPMatrix = Statics::SceneCamera.GetProjectionMatrix() * lightViewMatrix;

	return lightVPMatrix;
}

Texture ShadowMap::GetShadowMapTexture()
{
	return m_DepthTexture;
}
