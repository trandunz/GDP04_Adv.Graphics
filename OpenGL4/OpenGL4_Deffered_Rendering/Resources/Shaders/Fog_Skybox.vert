// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Fog_Skybox.vert
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec3 l_position;

// Output data for next Shader
out vec3 TexCoords;
out vec3 WorldPos;

// Outside Variables Passed In As 'Uniforms'
uniform mat4 PVMMatrix;
uniform mat4 Model;

// Main function that gets called per vertex.
void main()
{
	WorldPos = (Model * vec4(l_position, 1.0f)).xyz;
    TexCoords = l_position;
	gl_Position = PVMMatrix * vec4(l_position,1.0f);
}