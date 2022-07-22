// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Skybox.frag
// Description : Generic fragment shader for handling skybox
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec3 TexCoords;

// Outside Variables Passed In As 'Uniforms'
uniform samplerCube Texture0;

// Main function that gets called per vertex fragment.
void main()
{
    FragColor = texture(Texture0, TexCoords);
}
