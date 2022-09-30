// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : GameObject.cpp 
// Description : GameObject Implementation File
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#include "GameObject.h"
#include "ShaderLoader.h"
#include "LightManager.h"
#include "Skybox.h"
#include "Physics.h"
#include "SceneManager.h"

GameObject::GameObject(glm::vec3 _position)
{
    // Set starting position
    SetTranslation(_position);
    m_StencilShaderID = ShaderLoader::CreateShader("Outline.vert", "UnlitColor_Fog.frag");
    m_NormalsShaderID = ShaderLoader::CreateShader("Normals3D.vert", "ShowNormals.geo", "UnlitColor.frag");
}

GameObject::~GameObject()
{
    if (m_Mesh)
        m_Mesh = nullptr;

    m_ActiveTextures.clear();
}

void GameObject::Movement_WASDEQ()
{
    // Grab keyboard input for moving Object With WASDQE
    m_Input = {};

    for (auto& key : Statics::Keymap)
    {
        if (key.second)
        {
            if (key.first == GLFW_KEY_W)
            {
                m_Input.z -= 1.0f;
            }
            if (key.first == GLFW_KEY_A)
            {
                m_Input.x -= 1.0f;
            }
            if (key.first == GLFW_KEY_S)
            {
                m_Input.z += 1.0f;
            }
            if (key.first == GLFW_KEY_D)
            {
                m_Input.x += 1.0f;
            }
            if (key.first == GLFW_KEY_Q)
            {
                m_Input.y -= 1.0f;
            }
            if (key.first == GLFW_KEY_E)
            {
                m_Input.y += 1.0f;
            }
            if (key.first == GLFW_KEY_Z)
            {
                m_Input.w -= 1.0f;
            }
            if (key.first == GLFW_KEY_C)
            {
                m_Input.w += 1.0f;
            }
        }
    }
    // Normalize the input vecor.
    glm::normalize(m_Input);
}

void GameObject::Movement_YGHJ()
{
    // Grab keyboard input for moving Object With YGHJ
    m_Input = {};

    for (auto& key : Statics::Keymap)
    {
        if (key.second)
        {
            if (key.first == GLFW_KEY_Y)
            {
                m_Input.z -= 1.0f;
            }
            if (key.first == GLFW_KEY_G)
            {
                m_Input.x -= 1.0f;
            }
            if (key.first == GLFW_KEY_H)
            {
                m_Input.z += 1.0f;
            }
            if (key.first == GLFW_KEY_J)
            {
                m_Input.x += 1.0f;
            }
        }
    }
    // Normalize the input vecor.
    glm::normalize(m_Input);
}

void GameObject::Update()
{
    // If player provides input, Translate the gameobject accordingly.
    if (glm::length((glm::vec3)m_Input) > 0)
        Translate(m_Input * m_MovementSpeed * Statics::DeltaTime);
    // If player provides Rotational input, rotate accordingly
    if (m_Input.w != 0)
        Rotate({ 0,1,0 }, m_Input.w * 100 * Statics::DeltaTime);
}

void GameObject::Draw()
{
    if (m_Mesh)
    {
        if (m_ShowNormals)
        {
            // Bind shader
            glUseProgram(m_NormalsShaderID);

            SetSingleColorUniforms(m_NormalsShaderID, {1,1,0});
            SetNormals3DVertUniforms(m_NormalsShaderID);

            // Draw the mesh
            m_Mesh->Draw();

            // Unbind shader
            glUseProgram(0);
        }

        // Bind shader
        glUseProgram(m_ShaderID);

        // If shader program is single texture
        if (m_ShaderLocation.vertShader == "SingleTexture.vert" &&
            m_ShaderLocation.fragShader == "UnlitColor.frag")
        {
            SetSingleTextureUniforms();
        }
        else if (m_ShaderLocation.vertShader == "SingleTexture.vert" && 
            m_ShaderLocation.fragShader == "SingleTexture.frag")
        {
            SetSingleTextureUniforms();
        }
        // Else if Vertex shader is 3D with Normals
        else if (m_ShaderLocation.vertShader == "Normals3D.vert")   
        {
            SetNormals3DVertUniforms(m_ShaderID);

            if (m_ShaderLocation.geoShader == "Explode.geo")
            {
                SetExplodeUniforms(m_ShaderID);
            }
        
            // If Frag Shader is Blinn_Phong Lighting
            if (m_ShaderLocation.fragShader == "BlinnFong3D.frag")
            {
                SetBlinnFong3DUniforms();
                SetRimLighingUniforms();
            }
            // Else if Frag Shader is Reflection
            else if (m_ShaderLocation.fragShader == "Reflection.frag")
            {
                SetReflectionUniforms();
            }
            // Else If Frag Shader Is ReflectionMap
            else if (m_ShaderLocation.fragShader == "ReflectionMap.frag")
            {
                SetReflectionMapUniforms();
            }
            // Else If Frag Shader IS Blinn_Phong Lighting With Reflection
            else if (m_ShaderLocation.fragShader == "BlinnFong3D_Reflection.frag")
            {
                SetBlinnFong3DUniforms();
                SetRimLighingUniforms();
                SetReflectionMapUniforms();
            }
            // Else if Frag shader is Single Texture
            else if (m_ShaderLocation.fragShader == "SingleTexture.frag")
            {
                SetSingleTextureUniforms();
            }
        }
        // Else if vertex shader is fog
        else if (m_ShaderLocation.vertShader == "Fog.vert")
        {
            SetFogUniforms();
            // If frag shader is Lit Fog
            if (m_ShaderLocation.fragShader == "Lit_Fog.frag")
            {
                SetBlinnFong3DUniforms();
                SetRimLighingUniforms();
            }
            // Else if frag shader is perlin moss
            else if (m_ShaderLocation.fragShader == "Perlin_Moss.frag")
            {
                SetMossUniforms();
            }
        }
        else if (m_ShaderLocation.vertShader == "PositionOnly.vert")
        {
            SetPositionOnlyUniforms();

            if (m_ShaderLocation.fragShader == "SingleTexture.frag")
            {
                SetSingleTextureUniforms();
            }
        }
        
        if (Statics::StencilTest && m_StencilOutline)
        {
            // Enable stencil test if active
            glEnable(GL_STENCIL_TEST);
            // Set front and back stencil test actions
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            // Set front and back function and reference value for stencil testing
            glStencilFunc(GL_ALWAYS, 1, 0xFF);
            // Enable writing too stencil mask
            glStencilMask(0xFF);
        }

        // Draw the mesh
        m_Mesh->Draw();
        
        // Unbind
        glUseProgram(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        if (Statics::StencilTest && m_StencilOutline)
        { 
            // Set front and back function and reference value for stencil testing
            glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
            // Disable writing to stencil mask 
            glStencilMask(0x00);
            
            glUseProgram(m_StencilShaderID);
            
            // Projection * View * Model Matrix
            ShaderLoader::SetUniformMatrix4fv(std::move(m_StencilShaderID), "PVMMatrix", Statics::SceneCamera.GetPVMatrix() * m_Transform.transform);

            // Set Model Matrix
            ShaderLoader::SetUniformMatrix4fv(std::move(m_StencilShaderID), "ModelMatrix", m_Transform.transform);

            // Set is Foggy
            ShaderLoader::SetUniform1i(std::move(m_StencilShaderID), "Foggy", Statics::Foggy);
            if (Statics::Foggy)
            {
                ShaderLoader::SetUniform1f(std::move(m_StencilShaderID), "FogStart", 5.0f);
                ShaderLoader::SetUniform1f(std::move(m_StencilShaderID), "FogDepth", 10.0f);
                ShaderLoader::SetUniform3fv(std::move(m_StencilShaderID), "CameraPos", Statics::SceneCamera.GetPosition());
                ShaderLoader::SetUniform4fv(std::move(m_StencilShaderID), "FogColor", { 0.5f, 0.5f, 0.5f, 1.0f });
            }
            
            // Draw Stencil Mesh
            m_Mesh->Draw();
            
            glStencilMask(0x00); // Disable writing to stencil mask 
            glDisable(GL_STENCIL_TEST); // Disable stencil test
            glStencilMask(0xFF); // Enable writing to stencil mask 
            
            // Unbind
            glUseProgram(0);
            glBindTexture(GL_TEXTURE_2D, 0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        }
    }
}

void GameObject::SetMesh(Mesh* _mesh)
{
    m_Mesh = _mesh;
}

Mesh* GameObject::GetMesh()
{
    if (m_Mesh)
        return m_Mesh;
    else
        return nullptr;
}

Transform GameObject::GetTransform() const
{
    return m_Transform;
}

void GameObject::SetTranslation(glm::vec3 _newPosition)
{
    m_Transform.translation = _newPosition;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::Translate(glm::vec3 _translation)
{
    m_Transform.translation += _translation;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::SetRotation(glm::vec3 _axis, float _degrees)
{
    m_Transform.rotation_axis = _axis;
    m_Transform.rotation_value = glm::radians(_degrees);
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::Rotate(glm::vec3 _axis, float _degrees)
{
    m_Transform.rotation_axis = _axis;
    m_Transform.rotation_value += glm::radians(_degrees);
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::SetScale(glm::vec3 _newScale)
{
    m_Transform.scale = _newScale;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::Scale(glm::vec3 _scaleFactor)
{
    m_Transform.scale *= _scaleFactor;
    UpdateModelValueOfTransform(m_Transform);
}

void GameObject::SetModel(glm::mat4 _newModel)
{
    m_Transform.transform = _newModel;
}

void GameObject::RotateAround(glm::vec3&& _position, glm::vec3&& _axis, float&& _degrees)
{
    // get direction to the position
    glm::vec3 direction = glm::abs(_position - m_Transform.translation);
    // Translate to wards it
    m_Transform.transform = glm::translate(m_Transform.transform, direction);
    // Rotate
    m_Transform.transform = glm::rotate(m_Transform.transform, _degrees, _axis);
    // Translate back
    m_Transform.transform = glm::translate(m_Transform.transform, -direction);
}

void GameObject::SetActiveTextures(std::vector<Texture> _textures)
{
    m_ActiveTextures = _textures;
}

std::vector<Texture> GameObject::GetActiveTextures()
{
    return m_ActiveTextures;
}

void GameObject::SetShader(std::string _vertexSource, std::string _fragmentSource)
{
    m_ShaderID = ShaderLoader::CreateShader(_vertexSource, _fragmentSource);
    m_ShaderLocation = { _vertexSource , "",_fragmentSource};
}

void GameObject::SetShader(std::string _vertexSource, std::string _geoSource, std::string _fragmentSource)
{
    m_ShaderID = ShaderLoader::CreateShader(_vertexSource, _geoSource,_fragmentSource);
    m_ShaderLocation = { _vertexSource , _geoSource, _fragmentSource };
}

GLuint GameObject::GetShader()
{
    return m_ShaderID;
}

void GameObject::ClearInputVector()
{
    m_Input = {};
}

void GameObject::SetRimLighting(bool _rimLighting)
{
    m_RimLighting = _rimLighting;
}

bool GameObject::RayIntersection(Ray _ray)
{
    bool intersection = false;
    // If a mesh is assigned
    if (m_Mesh)
    {
        // Get refrence too indices and vertices
        std::vector<Vertex>* vertices = &m_Mesh->GetVertices();
        std::vector<unsigned>* indices = &m_Mesh->GetIndices();

        int index{ 0 };
        unsigned element{ 0 };
        // Loop over all indices and get points for each triangle in vertices
        for (unsigned i = 0; i < indices->size() / 3; i++)
        {
            element = (*indices)[index];
            glm::vec4 point1{ (*vertices)[element].position, 1.0f };
            point1 = m_Transform.transform * point1;

            element = (*indices)[index + 1];
            glm::vec4 point2{ (*vertices)[element].position, 1.0f };
            point2 = m_Transform.transform * point2;

            element = (*indices)[index + 2];
            glm::vec4 point3{ (*vertices)[element].position, 1.0f };
            point3 = m_Transform.transform * point3;

            // test for intersection with triangle
            intersection = Physics::IntersectTriangle(_ray, point1, point2, point3);
            if (intersection)
                return true;

            index += 3;
        }
        vertices = nullptr;
        indices = nullptr;
    }

    return intersection;
}

bool GameObject::RayIntersection(Ray _ray, glm::vec3& _point)
{
    bool intersection = false;
    // If a mesh is assigned
    if (m_Mesh)
    {
        // Get refrence too indices and vertices
        std::vector<Vertex>* vertices = &m_Mesh->GetVertices();
        std::vector<unsigned>* indices = &m_Mesh->GetIndices();

        int index{ 0 };
        unsigned element{ 0 };
        // Loop over all indices and get points for each triangle in vertices
        for (unsigned i = 0; i < indices->size() / 3; i++)
        {
            element = (*indices)[index];
            glm::vec4 point1{ (*vertices)[element].position, 1.0f };
            point1 = m_Transform.transform * point1;

            element = (*indices)[index + 1];
            glm::vec4 point2{ (*vertices)[element].position, 1.0f };
            point2 = m_Transform.transform * point2;

            element = (*indices)[index + 2];
            glm::vec4 point3{ (*vertices)[element].position, 1.0f };
            point3 = m_Transform.transform * point3;

            // test for intersection with triangle
            intersection = Physics::IntersectTriangle(_ray, point1, point2, point3, _point);
            if (intersection)
                return true;

            index += 3;
        }
        vertices = nullptr;
        indices = nullptr;
    }

    return intersection;
}

void GameObject::SetStencilOutlineActive(bool _outline)
{
    m_StencilOutline = _outline;
}

void GameObject::SetBlinnFong3DUniforms()
{
    // Apply Texture
    if (m_ActiveTextures.size() > 0)
    {
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[0].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
    }

    // Set Global Ambient Colour And Strength
    ShaderLoader::SetUniform1f(std::move(m_ShaderID), "AmbientStrength", 0.15f);
    ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "AmbientColor", { 1.0f,1.0f,1.0f });

    // Set Shininess
    ShaderLoader::SetUniform1f(std::move(m_ShaderID), "Shininess", 32.0f * 5);
    
    // Set Camera Position
    ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", Statics::SceneCamera.GetPosition());

    // Set Point Light Uniforms From Light Manager
    std::vector<PointLight>& pointLights = LightManager::GetInstance().GetPointLights();
    ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfPointLights", (int)pointLights.size());
    for (unsigned i = 0; i < pointLights.size(); i++)
    {
        ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].Position", pointLights[i].Position);
        ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "PointLights[" + std::to_string(i) + "].Color", pointLights[i].Color);
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
        ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].Color", directionalLights[i].Color);
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "DirectionalLights[" + std::to_string(i) + "].SpecularStrength", directionalLights[i].SpecularStrength);
    }

    // Set Spotlight Uniforms From Light Manager
    std::vector<SpotLight>& spotLights = LightManager::GetInstance().GetSpotLights();
    ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NumberOfSpotLights", (int)spotLights.size());
    for (unsigned i = 0; i < spotLights.size(); i++)
    {
        // If the spotlight is attached to the camera, Set Uniforms Accordingly
        if (spotLights[i].IsAttachedToCamera)
        {
            ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Position", Statics::SceneCamera.GetPosition());
            ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Direction", Statics::SceneCamera.GetFront());
        }
        // Else Apply Assigned Starting Positon And Direction
        else
        {
            ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Position", spotLights[i].Position);
            ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Direction", spotLights[i].Direction);
        }
        ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Color", spotLights[i].Color);
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].SpecularStrength", spotLights[i].SpecularStrength);
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].AttenuationLinear", spotLights[i].AttenuationLinear);
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].AttenuationExponent", spotLights[i].AttenuationExponent);
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].Cutoff", glm::cos(glm::radians(spotLights[i].Cutoff)));
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "SpotLights[" + std::to_string(i) + "].OuterCutoff", glm::cos(glm::radians(spotLights[i].OuterCutoff)));
    }
}

void GameObject::SetRimLighingUniforms()
{
    // Set Rim Lighting Bool
    ShaderLoader::SetUniform1i(std::move(m_ShaderID), "bRimLighting", m_RimLighting);

    // If RimLighting Enabled Then Set Colour And Exponent
    if (m_RimLighting)
    {
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "RimExponent", 4.0f);
        ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "RimColor", { 1.0f,0.0f,0.0f });
    }
}

void GameObject::SetReflectionUniforms()
{
    // Set Camera Position
    ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", Statics::SceneCamera.GetPosition());

    // Bind And Set Skybox Texture Uniform
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::GetInstance().GetTextureID().ID);
    ShaderLoader::SetUniform1i(std::move(m_ShaderID), "SkyboxTexture", 0);
}

void GameObject::SetReflectionMapUniforms()
{
    // If two or more textures are present
    if (m_ActiveTextures.size() > 1)
    {
        // Set Regular Texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[0].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
        // Set Reflection Map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[1].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "ReflectionMap", 1);
    }
    // Set Camera Pos
    ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", Statics::SceneCamera.GetPosition());

    // Bind And Set Skybox Texture Uniform
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_CUBE_MAP, Skybox::GetInstance().GetTextureID().ID);
    ShaderLoader::SetUniform1i(std::move(m_ShaderID), "SkyboxTexture", 2);
}

void GameObject::BillboardObjectToCamera(glm::vec3 _relativePos, glm::vec3 _scale)
{
    SetModel(glm::scale(glm::inverse(glm::lookAt(Statics::SceneCamera.GetPosition() + _relativePos, Statics::SceneCamera.GetPosition(), Statics::SceneCamera.GetUp())), _scale));
}

void GameObject::SetShowNormals(bool _showNormals)
{
    m_ShowNormals = _showNormals;
}

void GameObject::SetNormals3DVertUniforms(GLuint _shaderID)
{
    // Projection * View * Model Matrix
    ShaderLoader::SetUniformMatrix4fv(std::move(_shaderID), "PVMMatrix", Statics::SceneCamera.GetPVMatrix() * m_Transform.transform);

    // Set Model Matrix
    ShaderLoader::SetUniformMatrix4fv(std::move(_shaderID), "ModelMatrix", m_Transform.transform);
}

void GameObject::SetSingleTextureUniforms()
{
    // Apply Texture
    if (m_ActiveTextures.size() > 0)
    {
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[0].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
    }

    // Projection * View * Model Matrix
    ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", Statics::SceneCamera.GetPVMatrix() * m_Transform.transform);
}

void GameObject::SetFogUniforms()
{
    // Apply Texture
    if (m_ActiveTextures.size() > 0)
    {
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 1);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[0].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Texture0", 0);
    }

    // Set Model Matrix
    ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "Model", m_Transform.transform);
    ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMatrix", Statics::SceneCamera.GetPVMatrix());

    ShaderLoader::SetUniform1i(std::move(m_ShaderID), "Foggy", Statics::Foggy);
    if (Statics::Foggy)
    {
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "FogStart", 5.0f);
        ShaderLoader::SetUniform1f(std::move(m_ShaderID), "FogDepth", 10.0f);
        ShaderLoader::SetUniform3fv(std::move(m_ShaderID), "CameraPos", Statics::SceneCamera.GetPosition());
        ShaderLoader::SetUniform4fv(std::move(m_ShaderID), "FogColor", { 0.5f, 0.5f, 0.5f, 1.0f });
    }
}

void GameObject::SetMossUniforms()
{
    // Apply Texture
    if (m_ActiveTextures.size() > 2)
    {
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureCount", 3);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[1].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "TextureMoss", 1);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, m_ActiveTextures[2].ID);
        ShaderLoader::SetUniform1i(std::move(m_ShaderID), "NoiseTexture", 2);
    }
    ShaderLoader::SetUniform1f(std::move(m_ShaderID), "ElapsedTime", SceneManager::GetTimeSinceLoad());
    ShaderLoader::SetUniform1f(std::move(m_ShaderID), "GrowTime", 15.0f);
}

void GameObject::SetPositionOnlyUniforms()
{
    // Projection * View * Model Matrix
    ShaderLoader::SetUniformMatrix4fv(std::move(m_ShaderID), "PVMMatrix", Statics::SceneCamera.GetPVMatrix() * m_Transform.transform);
}

void GameObject::SetExplodeUniforms(GLuint _shaderID)
{
    ShaderLoader::SetUniform1f(std::move(_shaderID), "ElapsedTime", glfwGetTime());
    ShaderLoader::SetUniform1f(std::move(_shaderID), "Magnitude", 1.0f);
}

void GameObject::SetSingleColorUniforms(GLuint _shaderID, glm::vec3 _color)
{
    ShaderLoader::SetUniform3fv(std::move(_shaderID), "Color", _color);
}
