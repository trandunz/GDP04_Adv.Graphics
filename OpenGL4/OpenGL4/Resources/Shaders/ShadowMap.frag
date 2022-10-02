// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : 
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core
layout(location = 0) out float depth;

// Main function that gets called per vertex fragment.
void main()
{
	depth = gl_FragCoord.z;
}