#pragma once
#include "GameObject.h"
class Player :
    public GameObject
{
public:
    Player(glm::vec3 _startPos = {});
    virtual ~Player();

    virtual void Update() override;

    void Movement();
    void SetGround(GameObject* _ground);

private:
    void AlignWithInput();
    void CheckForGrounded();
    void HandleJump();

    bool m_IsMoving{};
    bool m_IsGrounded{};
    float m_MoveSpeed{ 10.0f };
    glm::vec3 m_Velocity{};
    Ray m_GroundedRay{};
    GameObject* m_Ground{ nullptr };
};

