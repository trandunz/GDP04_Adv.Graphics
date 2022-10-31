// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : ShadowMap.vert
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoords;

uniform mat4 Model;
uniform mat4 LightVPMatrix;

out VS_GS_VERTEX
{
	out vec4 Position;
	out vec4 AttribPosition;
	out mat4 PVMMatrix;
	out mat4 LightVPMatrix;
	out mat4 Model;
}vs_out;

out vec2 TexCoords;
out vec3 Position;

// Main function that gets called per vertex fragment.
void main()
{
	Position = l_position;
	TexCoords = l_texCoords;
    gl_Position = LightVPMatrix * Model * vec4(l_position, 1.0f);

	vs_out.AttribPosition = vec4(l_position, 1.0f);
	vs_out.Model = Model;
	vs_out.LightVPMatrix = LightVPMatrix;
	vs_out.Position = gl_Position;
	vs_out.PVMMatrix = LightVPMatrix * Model;
}
