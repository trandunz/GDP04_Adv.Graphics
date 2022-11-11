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

// Main function that gets called per vertex.
void main()
{
	gl_Position = vec4(l_position,1.0f);
}