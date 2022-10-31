// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Framebuffer.frag
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

// Input from Previous Shader
in vec2 TexCoords;
in vec3 Position;
in vec3 Normals;

// Outside Variables Passed In As 'Uniforms'
uniform float SpecularStrength;
uniform sampler2D Albedo;

// Main function that gets called per vertex fragment.
void main()
{
    gPosition = Position;
    gNormal = Normals;
    gAlbedoSpec.rgb = texture(Albedo,TexCoords).rgb;
    gAlbedoSpec.a = SpecularStrength;
}
