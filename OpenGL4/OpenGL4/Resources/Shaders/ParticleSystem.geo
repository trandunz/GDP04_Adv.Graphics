// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : PointToStar.geo
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 6) out;

out vec2 TexCoords;
out float LifeTime;

uniform vec3 vQuad1, vQuad2;

in VS_GS_VERTEX
{
	in vec3 AttribPosition;
	in mat4 PVMMatrix;
	in float LifeTime;
}gs_in[];

void main()
{
	gl_Position = gs_in[0].PVMMatrix * vec4((gs_in[0].AttribPosition + (-vQuad1 + vQuad2)),1.0f);
	TexCoords = vec2(0.0f, 1.0f);
	LifeTime = gs_in[0].LifeTime;
	EmitVertex();

	gl_Position = gs_in[0].PVMMatrix * vec4((gs_in[0].AttribPosition + (-vQuad1 - vQuad2)),1.0f);
	TexCoords = vec2(0.0f, 0.0f);
	LifeTime = gs_in[0].LifeTime;
	EmitVertex();

	gl_Position = gs_in[0].PVMMatrix * vec4((gs_in[0].AttribPosition +  (vQuad1 - vQuad2)),1.0f);
	TexCoords = vec2(1.0f, 0.0f);
	LifeTime = gs_in[0].LifeTime;
	EmitVertex();

	gl_Position = gs_in[0].PVMMatrix * vec4((gs_in[0].AttribPosition +  (-vQuad1 + vQuad2)),1.0f);
	TexCoords = vec2(0.0f, 1.0f);
	LifeTime = gs_in[0].LifeTime;
	EmitVertex();

	gl_Position = gs_in[0].PVMMatrix * vec4((gs_in[0].AttribPosition +  (vQuad1 - vQuad2)),1.0f);
	TexCoords = vec2(1.0f, 0.0f);
	LifeTime = gs_in[0].LifeTime;
	EmitVertex();

	gl_Position = gs_in[0].PVMMatrix * vec4((gs_in[0].AttribPosition +  (vQuad1 + vQuad2)),1.0f);
	TexCoords = vec2(1.0f, 1.0f);
	LifeTime = gs_in[0].LifeTime;
	EmitVertex();
}
