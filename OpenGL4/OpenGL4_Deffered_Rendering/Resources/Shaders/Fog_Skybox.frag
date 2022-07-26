// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Fog_Skybox.frag
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec3 TexCoords;
in vec3 WorldPos;

// Outside Variables Passed In As 'Uniforms'
uniform samplerCube Texture0;
uniform bool Foggy;
uniform float FogStart;
uniform float FogDepth;
uniform vec3 CameraPos;
uniform vec4 FogColor;

// Main function that gets called per vertex fragment.
void main()
{
    if (Foggy == true)
    {
        float d = distance(WorldPos, CameraPos);
        float contribution = (d - FogStart) / FogDepth;
        contribution = clamp(contribution, 0.0f, 1.0f);

        FragColor = mix(texture(Texture0, TexCoords), FogColor, contribution);
    }
    else
    {
        FragColor = texture(Texture0, TexCoords);
    }
}
