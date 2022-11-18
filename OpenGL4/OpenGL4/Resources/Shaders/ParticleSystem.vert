// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : PositionOnly.vert
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec4 l_position;

uniform mat4 PVMMatrix;

out float LifeTime; 

out VS_GS_VERTEX
{
	out vec3 AttribPosition;
	out mat4 PVMMatrix;
	out float LifeTime;
}vs_out;

// Main function that gets called per vertex.
void main()
{
	vs_out.PVMMatrix = PVMMatrix;
	vs_out.AttribPosition = l_position.xyz;
	vs_out.LifeTime = l_position.w;
	gl_Position = PVMMatrix vec4(l_position.xyz,1.0f);
	LifeTime = l_position.w;
}