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

// Output data for next Shader
out vec2 TexCoords;

uniform mat4 Model;

// Main function that gets called per vertex.
void main()
{
    TexCoords = l_texCoords;
	gl_Position = Model * vec4(l_position,1.0f);
}