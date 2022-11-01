// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : FrameBuffer.cpp 
// Description : FrameBuffer Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "FrameBuffer.h"
#include "TextureLoader.h"
#include "ShaderLoader.h"
#include "StaticMesh.h"
#include "ShadowMap.h"
#include "gBuffer.h"
#include "LightManager.h"

FrameBuffer::FrameBuffer()
{
	m_Transform.scale = { 2,2,2 };
	UpdateModelValueOfTransform(m_Transform);

	// Create shaders
	m_ShaderID = ShaderLoader::CreateShader("Framebuffer.vert", "Framebuffer.frag");
	m_RainShaderID = ShaderLoader::CreateShader("Framebuffer.vert", "Framebuffer_Rain.frag");
	m_CAShaderID = ShaderLoader::CreateShader("Framebuffer.vert", "Framebuffer_CA.frag");
	m_CRTShaderID = ShaderLoader::CreateShader("Framebuffer.vert", "Framebuffer_CRT.frag");

	// Create textures
	m_RenderTexture = TextureLoader::CreateRenderTexture();
	m_RainTexture = TextureLoader::LoadTexture("Rain.png");

	if (Statics::DSA)
	{
		// Create frame buffers and render buffers
		glCreateFramebuffers(1, &m_FrameBufferID);
		glCreateRenderbuffers(1, &m_RenderBufferID);
		// Set frame buffer attachment
		glNamedFramebufferTexture(m_FrameBufferID, GL_COLOR_ATTACHMENT0, m_RenderTexture.ID, 0);
		// Set render buffer storage type
		glNamedRenderbufferStorage(m_RenderBufferID, GL_DEPTH24_STENCIL8, Statics::WindowSize.x, Statics::WindowSize.y);
		// Set render buffer attachment
		glNamedFramebufferRenderbuffer(m_FrameBufferID, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);

		auto status = glCheckNamedFramebufferStatus(m_FrameBufferID, GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			Print("Framebuffer Error: " + std::to_string(status));

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		// Generate frame buffer
		glGenFramebuffers(1, &m_FrameBufferID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

		// Set frame buffer attachment
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_RenderTexture.ID, 0);

		// Generate render buffer
		glGenRenderbuffers(1, &m_RenderBufferID);
		glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferID);
		// Set render buffer storage type
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Statics::WindowSize.x, Statics::WindowSize.y);
		// Set render buffer attachment
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferID);

		auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
			Print("Framebuffer Error: " + std::to_string(status));

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

FrameBuffer::~FrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDeleteFramebuffers(1, &m_FrameBufferID);
	glDeleteRenderbuffers(1, &m_RenderBufferID);
}

void FrameBuffer::Bind()
{
	// Bind frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
	if (Statics::DSA)
	{
		GLfloat clearColor[] = { 0.0f,0.0f,0.0f, 1.0f };
		glClearNamedFramebufferfv(m_FrameBufferID, GL_COLOR, 0, clearColor);
	}
	glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::Unbind()
{
	// unbind frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);

	// bind appropriate program and set corresponding uniforms
	if (m_EnableRain)
	{
		glUseProgram(m_RainShaderID);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_RainTexture.ID);
		ShaderLoader::SetUniform1i(std::move(m_RainShaderID), "Texture1", 1);
		ShaderLoader::SetUniform1f(std::move(m_RainShaderID), "ElapsedTime", (float)glfwGetTime());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RenderTexture.ID);
		ShaderLoader::SetUniform1i(std::move(m_RainShaderID), "Texture0", 0);
		ShaderLoader::SetUniformMatrix4fv(std::move(m_RainShaderID), "Model", m_Transform.transform);

	}
	else if (m_EnableCA)
	{
		glUseProgram(m_CAShaderID);
		glm::dvec2 cursorPos{};
		glfwGetCursorPos(Statics::RenderWindow, &cursorPos.x, &cursorPos.y);
		ShaderLoader::SetUniform2f(std::move(m_CAShaderID), "MousePos", (GLfloat)cursorPos.x, (GLfloat)cursorPos.y);
		ShaderLoader::SetUniform2f(std::move(m_CAShaderID), "Resolution", (GLfloat)Statics::WindowSize.x, (GLfloat)Statics::WindowSize.y);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RenderTexture.ID);
		ShaderLoader::SetUniform1i(std::move(m_CAShaderID), "Texture0", 0);
		ShaderLoader::SetUniformMatrix4fv(std::move(m_CAShaderID), "Model", m_Transform.transform);
	}
	else if (m_EnableCRT)
	{
		glUseProgram(m_CRTShaderID);

		ShaderLoader::SetUniform2f(std::move(m_CRTShaderID), "WindowSize", (GLfloat)Statics::WindowSize.x, (GLfloat)Statics::WindowSize.y);
		ShaderLoader::SetUniform1f(std::move(m_RainShaderID), "ElapsedTime", (float)glfwGetTime());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RenderTexture.ID);
		ShaderLoader::SetUniform1i(std::move(m_CRTShaderID), "Texture0", 0);
		ShaderLoader::SetUniformMatrix4fv(std::move(m_CRTShaderID), "Model", m_Transform.transform);
	}
	else
	{
		glUseProgram(m_ShaderID);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_RenderTexture.ID);
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
		ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "Model", m_Transform.transform);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gBuffer::GetInstance().GetPositionTexture().ID);
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "gPosition", 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, gBuffer::GetInstance().GetNormalsTexture().ID);
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "gNormals", 2);

		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, gBuffer::GetInstance().GetAlbedoTexture().ID);
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "gAlbedoSpec", 3);

		ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", Statics::SceneCamera.GetPosition());
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Shininess", 32.0f * 5);
		ShaderLoader::SetUniform1f(std::move(m_ShaderID), "AmbientStrength", 0.15f);
		ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "AmbientColor", { 1.0f,1.0f,1.0f });

		// Set Point Light Uniforms From Light Manager
		std::vector<PointLight>& pointLights = LightManager::GetInstance().GetPointLights();
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfPointLights", (int)pointLights.size());
		for (unsigned i = 0; i < pointLights.size(); i++)
		{
			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].Position", pointLights[i].Position);
			ShaderLoader::SetUniform4fv(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].Color", pointLights[i].Color);
			ShaderLoader::SetUniform1f(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].SpecularStrength", pointLights[i].SpecularStrength);
			ShaderLoader::SetUniform1f(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].AttenuationLinear", pointLights[i].AttenuationLinear);
			ShaderLoader::SetUniform1f(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].AttenuationExponent", pointLights[i].AttenuationExponent);
		}

		// Set Directional Light Uniforms From Light Manager
		std::vector<DirectionalLight>& directionalLights = LightManager::GetInstance().GetDirectionalLights();
		ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfDirectionalLights", (int)directionalLights.size());
		for (unsigned i = 0; i < directionalLights.size(); i++)
		{
			ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].Direction", directionalLights[i].Direction);
			ShaderLoader::SetUniform4fv(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].Color", directionalLights[i].Color);
			ShaderLoader::SetUniform1f(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].SpecularStrength", directionalLights[i].SpecularStrength);
		}
	}

	// Draw the quad
	StaticMesh::Triangle_Quad->Draw();

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FrameBuffer::SetShader(std::string _vert, std::string _frag)
{
	m_ShaderID = ShaderLoader::CreateShader(_vert, _frag);
}

void FrameBuffer::EnableRain()
{
	m_EnableRain = true;
}

void FrameBuffer::EnableCA()
{
	m_EnableCA = true;
}

void FrameBuffer::EnableCRT()
{
	m_EnableCRT = true;
}

void FrameBuffer::DisableAllEffects()
{
	m_EnableCRT = false;
	m_EnableCA = false;
	m_EnableRain = false;
}
