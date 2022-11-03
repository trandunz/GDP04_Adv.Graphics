// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Framebuffer.vert
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoords;

uniform mat4 PVMMatrix;

out VS_GS_VERTEX
{
	out vec3 AttribPosition;
	out mat4 PVMMatrix;
}vs_out;

// Main function that gets called per vertex.
void main()
{
	vs_out.PVMMatrix = PVMMatrix;
	vs_out.AttribPosition = l_position;
	gl_Position = vec4(l_position,1.0f);
}