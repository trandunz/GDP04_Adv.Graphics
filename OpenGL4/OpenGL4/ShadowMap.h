#pragma once
#include "Statics.h"
class ShadowMap
{
public:
	static ShadowMap& GetInstance()
	{
		static ShadowMap shadowMap{};
		return shadowMap;
	}

	ShadowMap();
	~ShadowMap();

	void Bind();
	void Unbind();

	void SetShader(ShaderProgramLocation _location);

	glm::mat4 GetLightVPMatrix();
	Texture GetShadowMapTexture();

private:
	Transform m_Transform{};
	Texture m_DepthTexture{};
	GLuint m_FrameBufferID{};
	GLuint m_ShaderID{};
};

