// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : SingleTexture.vert
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec3 l_position;

out VS_GS_VERTEX
{
	out vec4 Position;
	out mat4 PVMMatrix;
}vs_out;

// Outside Variables Passed In As 'Uniforms'
uniform mat4 PVMMatrix;

// Main function that gets called per vertex.
void main()
{
	gl_Position = PVMMatrix * vec4(l_position,1.0f);

	vs_out.Position = gl_Position;
	vs_out.PVMMatrix = PVMMatrix;
}