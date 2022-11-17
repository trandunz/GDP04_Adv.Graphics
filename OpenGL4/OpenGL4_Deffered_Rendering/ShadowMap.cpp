// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : ShadowMap.cpp 
// Description : ShadowMap Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "ShadowMap.h"
#include "StaticMesh.h"
#include "ShaderLoader.h"
#include "LightManager.h"

ShadowMap::ShadowMap()
{
	// make the depth texture
	glGenTextures(1, &m_DepthTexture.ID);
	glBindTexture(GL_TEXTURE_2D, m_DepthTexture.ID);

	// set the size and component type
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 4096, 4096, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	
	// Set texture parameter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// generate the frame buffer
	glGenFramebuffers(1, &m_FrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

	// det the frame buffer attachment
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
	// bind the frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glViewport(0, 0, 4096, 4096);
}

void ShadowMap::Unbind()
{
	// unbind the frame buffer
	glFlush();
	glFinish();
	glDisable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glViewport(0,0,Statics::WindowSize.x, Statics::WindowSize.y);
}

glm::mat4 ShadowMap::GetLightVPMatrix()
{
	//glm::mat4 orthogonalProjection = glm::ortho(-300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 75.0f);

	// Get the view matrix from the point light
	auto& pointLights = LightManager::GetInstance().GetPointLights();
	glm::mat4 lightViewMatrix{};
	if (pointLights.size() > 0)
	{
		lightViewMatrix = glm::lookAt
		(
			LightManager::GetInstance().GetPointLights()[0].Position,
			glm::vec3{ 0.0f,0.0f,0.0f },
			glm::vec3{ 0.0f,0.0f,1.0f }
		);
	}

	// times it by the cameras projection matrix
	glm::mat4 lightVPMatrix = Statics::ActiveCamera.GetProjectionMatrix() * lightViewMatrix;

	return lightVPMatrix;
}

Texture ShadowMap::GetShadowMapTexture()
{
	return m_DepthTexture;
}
