#include "ShadowMap.h"
#include "StaticMesh.h"
#include "ShaderLoader.h"
#include "LightManager.h"

ShadowMap::ShadowMap()
{
	glGenTextures(1, &m_DepthTexture.ID);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture.ID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Statics::WindowSize.x, Statics::WindowSize.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

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
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 ShadowMap::GetLightVPMatrix()
{
	glm::mat4 lightViewMatrix = glm::lookAt
	(
		glm::vec3{0.0f,-1.0f,0.0f },
		glm::vec3{0.0f,0.0f,0.0f},
		glm::vec3{0.0f,1.0f,0.0f}
	);

	glm::mat4 lightVPMatrix = Statics::SceneCamera.GetProjectionMatrix() * lightViewMatrix;

	return lightVPMatrix;
}

Texture ShadowMap::GetShadowMapTexture()
{
	return m_DepthTexture;
}
