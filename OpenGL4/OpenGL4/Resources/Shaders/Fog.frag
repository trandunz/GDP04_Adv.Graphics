#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec3 TexCoords;
in vec4 WorldPos;

// Outside Variables Passed In As 'Uniforms'
uniform sampler2D Texture0;
uniform float FogStart;
uniform float FogDepth;
uniform vec3 CameraPos;
uniform vec3 FogColor;

// Main function that gets called per vertex fragment.
void main()
{
    float d = distance(WorldPos.xyz, CameraPos);
    float contribution = (d - FogStart) / FogDepth;
    contribution = clamp(contribution, 0.0f, 1.0f);

    FragColor = mix(texture(Texture0, TexCoords), FogColor, contribution);
}
