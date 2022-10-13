#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_GS_VERTEX
{
	in vec4 Position;
	in mat4 PVMMatrix;
	in vec3 Normals;
	in vec2 TexCoords;
}gs_in[];

out vec2 TexCoords;

uniform mat4 PVMatrix;
uniform vec3 Points[3];

void main()
{
    gl_Position = PVMatrix * vec4(Points[0], 1.0f);
    TexCoords = gs_in[0].TexCoords;
    EmitVertex();
    gl_Position = PVMatrix * vec4(Points[1], 1.0f);
    TexCoords = gs_in[1].TexCoords;
    EmitVertex();
    gl_Position = PVMatrix * vec4(Points[2], 1.0f);
    TexCoords = gs_in[2].TexCoords;
    EmitVertex();

	EndPrimitive();
}
