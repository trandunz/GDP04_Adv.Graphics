#version 460 core

// Output to C++
layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoords;
layout (location = 2) in vec3 l_normals;

// Output data for next Shader
out vec2 TexCoords;
out vec4 WorldPos;
out vec3 Position;
out vec3 Normals;

uniform mat4 Model;
uniform mat4 PVMatrix;

// Main function that gets called per vertex fragment.
void main()
{
	TexCoords = l_texCoords;
	Normals = normalize(mat3(transpose(inverse(Model))) * l_normals);
    WorldPos = Model * vec4(l_position, 1.0f);
	Position = WorldPos.xyz;
    gl_Position = PVMatrix * WorldPos;
}
