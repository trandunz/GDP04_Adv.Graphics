// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : UnlitColor.frag
// Description : Generic fragment shader for handling unlit coloured Meshes
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

in vec3 Position;

// Outside Variables Passed In As 'Uniforms'
uniform vec3 Color;
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
        float d = distance(Position, CameraPos);
        float contribution = (d - FogStart) / FogDepth;
        contribution = clamp(contribution, 0.0f, 1.0f);

        FragColor = mix(vec4(1.0f,0.0f,0.0f,1.0f), FogColor, contribution);
    }
    else
    {
        FragColor = vec4(1.0f,0.0f,0.0f,1.0f);
    }
}
