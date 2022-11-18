// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Player.h 
// Description : Player Header File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#pragma once
#include "GameObject.h"
class Player :
    public GameObject
{
public:
    /// <summary>
    /// Player constructor
    /// </summary>
    /// <param name="_startPos"></param>
    Player(glm::vec3 _startPos = {});
    /// <summary>
    /// Player destructor
    /// </summary>
    virtual ~Player();

    /// <summary>
    /// Player update
    /// </summary>
    virtual void Update() override;

    /// <summary>
    /// Player movement
    /// </summary>
    void Movement();
    /// <summary>
    /// Sets the ground object for the player
    /// </summary>
    /// <param name="_ground"></param>
    void SetGround(GameObject* _ground);

    /// <summary>
    /// Toggles the thirdperson camera
    /// </summary>
    /// <param name="_thirdPersonCamera"></param>
    void ToggleThirdPersonCamera(bool _thirdPersonCamera);
private:
    /// <summary>
    /// Aligns the players rotation with input or camera if third person
    /// </summary>
    void AlignWithInput();
    /// <summary>
    /// Sets is grounded to tru if raycast from player downward hits ground object
    /// </summary>
    void CheckForGrounded();
    /// <summary>
    /// Handles player jumping
    /// </summary>
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

