#pragma once
#include "Scene.h"
#include "GameObject.h"
#include "Skybox.h"
#include "Firework.h"
#include "C_Particle_System.h"

class Scene_Assignment3 :
    public Scene
{
public:
	Scene_Assignment3();
	~Scene_Assignment3();

	virtual void Start() override;
	virtual void Update() override;
	virtual void KeyEvents() override;
	virtual void CursorMoveEvent(double& xpos, double& ypos) override;
	virtual void CursorClickEvent(int button, int action, int mods) override;
	virtual void Draw() override;

private:
	void AlignPlayerWithInput();

	glm::vec2 m_CursorPos{};

	SkinnedMesh* m_AnimatedModel{ nullptr };
	SkinnedMesh* m_BonfireModel{ nullptr };
	SkinnedMesh* m_FountainModel{ nullptr };
	Animator* m_Animator{ nullptr };
	Animation* m_DanceAnim{ nullptr };

	bool m_CharacterMoving{};

	ParticleSystem* m_ParticleSystem{ nullptr };
	ParticleSystem* m_SmokeSystem{ nullptr };
	C_Particle_System* m_FountainSystem{ nullptr };
	C_Particle_System* m_SnowSystem{ nullptr };

	std::vector< Firework*> m_Fireworks{};

	GameObject* m_Fountain{ nullptr };
	GameObject* m_AssimpObject{ nullptr };
	GameObject* m_Plane{ nullptr };
	GameObject* m_Bonfire{ nullptr };
};

