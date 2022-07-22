// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Reflection.frag
// Description : Generic fragment shader for handling Reflection
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;
in vec3 Position;
in vec3 Normals;

// Outside Variables Passed In As 'Uniforms'
uniform samplerCube SkyboxTexture;
uniform vec3 CameraPos;

// Main function that gets called per vertex fragment.
void main()
{
    vec3 viewDir = normalize(Position - CameraPos);
    vec3 reflectedDir = reflect(viewDir, Normals);
    FragColor = texture(SkyboxTexture, reflectedDir);
}
