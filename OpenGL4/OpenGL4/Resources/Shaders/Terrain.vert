// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Normals3D.vert
// Description : Generic vertex shader for handling position, texture coordinate and Normal layouts
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoords;
layout (location = 2) in vec3 l_normals;

// Output data for next Shader
out vec2 TexCoords;
out vec3 Position;
out vec3 Normals;
out float Height;

// Outside Variables Passed In As 'Uniforms'
uniform mat4 PVMMatrix;
uniform mat4 ModelMatrix;

// Main function that gets called per vertex.
void main()
{
    TexCoords = l_texCoords * 513;
	Height = l_position.y;
	Normals = normalize(mat3(transpose(inverse(ModelMatrix))) * l_normals);
	Position = vec3(ModelMatrix * vec4(l_position, 1.0f));
	gl_Position = PVMMatrix * vec4(l_position,1.0f);
}