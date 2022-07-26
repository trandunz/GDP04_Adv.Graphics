// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : UnlitColor.frag
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Outside Variables Passed In As 'Uniforms'
uniform vec4 Color;

// Main function that gets called per vertex fragment.
void main()
{
    FragColor = Color;
}
