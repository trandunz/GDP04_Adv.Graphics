// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : ShowNormals.geo
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

out vec2 TexCoords;

in VS_GS_VERTEX
{
	in vec4 Position;
	in vec4 LocalPos;
	in mat4 PVMMatrix;
	in vec3 Normals;
	in vec2 TexCoords;
}gs_in[];

void main()
{
	// point 1: on the shape
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	// point 2: point on shape + normal * some distance (0.05f)
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(gs_in[0].Normals * 0.05f, 0.0f);
	EmitVertex();

	EndPrimitive();
}
