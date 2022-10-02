#pragma once
#include "Statics.h"
class ShadowMap
{
public:
	static ShadowMap& GetInstance()
	{
		static ShadowMap shadowMap;
		return shadowMap;
	}

	ShadowMap();
	~ShadowMap();

	void Bind();
	void Unbind();

	glm::mat4 GetLightVPMatrix();
	Texture GetShadowMapTexture();

private:
	Texture m_DepthTexture{};
	GLuint m_FrameBufferID{};
};

