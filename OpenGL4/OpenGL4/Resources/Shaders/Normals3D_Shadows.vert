// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Normals3D.vert
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoords;
layout (location = 2) in vec3 l_normals;

// Output data for next Shader
out vec2 TexCoords;
out vec3 Position;
out vec3 Normals;
out vec4 FragPosLightSpace;

out VS_GS_VERTEX
{
	out vec4 Position;
	out vec4 LocalPos;
	out mat4 PVMMatrix;
	out vec3 Normals;
	out vec2 TexCoords;
}vs_out;

// Outside Variables Passed In As 'Uniforms'
uniform mat4 PVMMatrix;
uniform mat4 ModelMatrix;
uniform mat4 LightVPMatrix;

// Main function that gets called per vertex.
void main()
{
    TexCoords = l_texCoords;
	Normals = normalize(mat3(transpose(inverse(ModelMatrix))) * l_normals);
	Position = vec3(ModelMatrix * vec4(l_position, 1.0f));
	
	FragPosLightSpace = LightVPMatrix * vec4(Position, 1.0f);
	
	gl_Position = PVMMatrix * vec4(l_position,1.0f);

	vs_out.LocalPos = vec4(l_position, 1.0f);
	vs_out.Position = gl_Position;
	vs_out.PVMMatrix = PVMMatrix;
	vs_out.Normals = Normals;
	vs_out.TexCoords = l_texCoords;
}