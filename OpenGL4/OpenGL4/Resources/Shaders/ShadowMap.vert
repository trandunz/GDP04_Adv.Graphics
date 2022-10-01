// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Fog.vert
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) in vec3 l_position;

uniform mat4 Model;
uniform mat4 LightVPMatrix;

// Main function that gets called per vertex fragment.
void main()
{
    gl_Position = LightVPMatrix * Model * vec4(l_position, 1.0f);
}
