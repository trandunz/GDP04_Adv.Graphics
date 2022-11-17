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

    void ToggleThirdPersonCamera(bool _thirdPersonCamera);
private:
    void AlignWithInput();
    void CheckForGrounded();
    void HandleJump();

    bool m_IsMoving{};
    bool m_IsGrounded{};
    bool m_IsThirdPerson{};
    float m_MoveSpeed{ 10.0f };
    glm::vec3 m_Velocity{};
    Ray m_GroundedRay{};
    GameObject* m_Ground{ nullptr };
    Camera m_ThirdPersonCamera{};
};

