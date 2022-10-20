// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Framebuffer_Rain.frag
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;

// Outside Variables Passed In As 'Uniforms'
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform float ElapsedTime;

// Main function that gets called per vertex fragment.
void main()
{
	vec3 raintex = texture(Texture1,vec2(TexCoords.x*2.0f,TexCoords.y*0.1f+ElapsedTime*0.125f)).rgb/8.0f;
	vec2 where = (TexCoords.xy-raintex.xy);
	vec3 texchur1 = texture(Texture0,vec2(where.x,where.y)).rgb;

	FragColor = vec4(texchur1,1.0f);
}
