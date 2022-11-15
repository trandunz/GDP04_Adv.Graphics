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
uniform mat4 PVMMatrix;

uniform vec3 vQuad1, vQuad2;

void main()
{
	vec3 point = gl_in[0].gl_Position.xyz + (-vQuad1 + vQuad2);
	gl_Position = PVMMatrix * vec4(point,1.0f);
	TexCoords = vec2(0.0f, 1.0f);
	EmitVertex();

	point = gl_in[0].gl_Position.xyz + (-vQuad1 - vQuad2);
	gl_Position = PVMMatrix * vec4(point,1.0f);
	TexCoords = vec2(0.0f, 0.0f);
	EmitVertex();

	point = gl_in[0].gl_Position.xyz + (vQuad1 - vQuad2);
	gl_Position = PVMMatrix * vec4(point,1.0f);
	TexCoords = vec2(1.0f, 0.0f);
	EmitVertex();

	point = gl_in[0].gl_Position.xyz + (-vQuad1 + vQuad2);
	gl_Position = PVMMatrix * vec4(point,1.0f);
	TexCoords = vec2(0.0f, 1.0f);
	EmitVertex();

	point = gl_in[0].gl_Position.xyz + (vQuad1 - vQuad2);
	gl_Position = PVMMatrix * vec4(point,1.0f);
	TexCoords = vec2(1.0f, 0.0f);
	EmitVertex();

	point = gl_in[0].gl_Position.xyz + (vQuad1 + vQuad2);
	gl_Position = PVMMatrix * vec4(point,1.0f);
	TexCoords = vec2(1.0f, 1.0f);
	EmitVertex();

	EndPrimitive();
}
