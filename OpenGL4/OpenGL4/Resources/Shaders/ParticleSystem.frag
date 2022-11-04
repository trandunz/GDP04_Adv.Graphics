// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : SingleTexture.frag
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in float LifeTime;

uniform vec4 ColorOverLifetime;

// Main function that gets called per vertex fragment.
void main()
{
    FragColor = mix(vec4( 0.0f, 0.48f, 0.58f, 1 ), ColorOverLifetime, 0);
}