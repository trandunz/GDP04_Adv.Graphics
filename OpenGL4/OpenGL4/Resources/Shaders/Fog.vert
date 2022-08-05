#version 460 core

// Output to C++
layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoords;
layout (location = 2) in vec3 l_normals;

// Input from Previous Shader
out vec3 TexCoords;
out vec4 WorldPos;

uniform Model;
uniform PVMatrix;

// Main function that gets called per vertex fragment.
void main()
{
    WorldPos = Model * vec4(l_position, 1.0f);
    gl_Position = PVMatrix * WorldPos;
    TexCoords = l_texCoords;
}
