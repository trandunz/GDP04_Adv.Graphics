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
	// Bottom Left Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(gs_in[0].Normals * 0.05f, 0.0f);
	EmitVertex();

	EndPrimitive();
}
