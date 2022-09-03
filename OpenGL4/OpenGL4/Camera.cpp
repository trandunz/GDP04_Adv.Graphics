// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Camera.cpp 
// Description : Camera Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "Camera.h"
#include "Statics.h"

Camera::Camera(glm::vec3 _position)
{
    m_Position = _position;
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
    // Return Projection Matrix Based On If Perspective Or Ortho
    return m_Perspective 
        ? 
        glm::perspective(
            glm::radians(m_Fov), 
            (float)Statics::WindowSize.x / 
            (float)Statics::WindowSize.y,
            m_NearPlane, 
            m_FarPlane) 
        : 
        glm::ortho(
            (float)-Statics::WindowSize.x / 2,
            (float)Statics::WindowSize.x / 2,
            (float)-Statics::WindowSize.y / 2,
            (float)Statics::WindowSize.y / 2,
            m_NearPlane, 
            m_FarPlane);
}

glm::vec3 Camera::GetFront()
{
    return m_Front;
}

glm::vec3 Camera::GetRight()
{
    return m_Right;
}

glm::vec3 Camera::GetUp()
{
    return m_Up;
}

void Camera::MoveForward()
{
    m_Position += m_Front * m_MoveSpeed * Statics::DeltaTime;
}

void Camera::MoveBackward()
{
    m_Position +=  -m_Front * m_MoveSpeed * Statics::DeltaTime;
}

void Camera::UpdateRotationVectors()
{
    // Set thee new front vector
    m_Front = glm::normalize(glm::vec3{
        cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw)),
        sin(glm::radians(m_Pitch)),
        cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw))
        });

    // Set New Right Vector
    m_Right = glm::normalize(glm::cross(m_Front, { 0,1,0 }));

    // Set new up Vector
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}

glm::mat4 Camera::GetPVMatrix()
{
    return GetProjectionMatrix() * GetViewMatrix();
}

void Camera::SetNearAndFarPlane(glm::vec2 _nearAndFar)
{
    m_NearPlane = _nearAndFar.x;
    m_FarPlane = _nearAndFar.y;
}

void Camera::SetLookSensitivity(float _newSenstivity)
{
    m_LookSensitivity = _newSenstivity;
}

float Camera::GetLookSensitivity()
{
    return m_LookSensitivity;
}

void Camera::Movement()
{
    UpdateRotationVectors();
    UpdatePosition();
}

void Camera::MouseLook(glm::vec2 _mousePos)
{
    // Reset Mouse Pos If First Time Moving Mouse
    if (m_LastMousePos == glm::vec2(0, 0))
        m_LastMousePos = _mousePos;

    // Update Pitch And Yaw
    m_Yaw += (m_LookSensitivity * (_mousePos.x - m_LastMousePos.x) * Statics::DeltaTime);
    m_Pitch += (m_LookSensitivity * (m_LastMousePos.y - _mousePos.y) * Statics::DeltaTime);

    // Clamp Pitch To 89 degrees
    m_Pitch = glm::clamp(m_Pitch, -89.0f, 89.0f);

    // Update Last Mouse Pos
    m_LastMousePos = _mousePos;
}

glm::vec3 Camera::GetRayCursorRayDirection(glm::vec2 _mousePos)
{
    // Convert mouse pos to normalize device coordinates
    glm::vec3 ndc
    {
        (2.0f * _mousePos.x) / Statics::WindowSize.x - 1.0f,
        1.0f - (2.0f * _mousePos.y) / Statics::WindowSize.y,
        1.0f
    };
    // Convert NDC to Clip Coordinates
    glm::vec4 clip { ndc.x, ndc.y, -1.0f, 1.0 };
    // Convert Clip Coords to camera / eye coordinates by undoing projection transformation
    glm::vec4 eye = glm::inverse(GetProjectionMatrix()) * clip;
    // Force eye coordinate to be a forward direction instead of a point
    eye = { eye.x, eye.y, -1.0f, 0.0 };
    // Convert eye coordinates to world coordinates by undoing view transformation
    glm::vec3 worldRay = glm::vec3(glm::inverse(GetViewMatrix()) * eye);
    // Normalize it so its only a direction
    return glm::normalize(worldRay);
}

void Camera::UpdatePosition()
{
    float x;
    float y;
    float z;

    // Scale Movement Speed On Projection Type
    if (m_Perspective)
    {
        // Scale With Move Speed
        x = m_InputVec.x * m_MoveSpeed;
        y = m_InputVec.y * m_MoveSpeed;
        z = m_InputVec.z * m_MoveSpeed;
    }
    else
    {
        // Scale With Move Speed * Window Size
        x = m_InputVec.x * m_MoveSpeed * Statics::WindowSize.x;
        y = m_InputVec.y * m_MoveSpeed * Statics::WindowSize.x;
        z = m_InputVec.z * m_MoveSpeed * Statics::WindowSize.x;
    }

    // Update position values if changed
    if (x != 0)
    {
        m_Position += m_Right * x * Statics::DeltaTime;
    }
    if (y != 0)
    {
        m_Position += Up * y * Statics::DeltaTime;
    }
    if (z != 0)
    {
        m_Position += m_Front * z * Statics::DeltaTime;
    }
}

void Camera::Movement_Capture()
{
    // Reset Input Vec
    m_InputVec = {};

    // Grab Input From Keymap
    for (auto& key : Statics::Keymap)
    {
        if (key.second == true)
        {
            switch (key.first)
            {
            case GLFW_KEY_D:
            {
                m_InputVec.x = 1;
                break;
            }
            case GLFW_KEY_A:
            {
                m_InputVec.x = -1;
                break;
            }
            case GLFW_KEY_Q:
            {
                m_InputVec.y = -1;
                break;
            }
            case GLFW_KEY_E:
            {
                m_InputVec.y = 1;
                break;
            }
            case GLFW_KEY_W:
            {
                m_InputVec.z = 1;
                break;
            }
            case GLFW_KEY_S:
            {
                m_InputVec.z = -1;
                break;
            }
            
            default:
                break;
            }
        }
    }

    // Normalize all input
    glm::normalize(m_InputVec);
}
