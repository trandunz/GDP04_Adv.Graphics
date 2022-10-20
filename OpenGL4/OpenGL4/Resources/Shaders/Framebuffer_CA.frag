// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Framebuffer_CA.frag
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;

// Outside Variables Passed In As 'Uniforms'
uniform sampler2D Texture0;
uniform vec2 MousePos;
uniform vec2 Resolution;

// Main function that gets called per vertex fragment.
void main()
{
    float aberrationAmount =  0.1f + abs(MousePos.y / Resolution.y / 8.0f);

	vec2 distFromCenter = TexCoords - 0.5f;

    // stronger aberration near the edges by raising to power 3
    vec2 aberrated = aberrationAmount * pow(distFromCenter, vec2(3.0f, 3.0f));
    
	FragColor = vec4
    (
    	texture(Texture0, TexCoords - aberrated).r,
    	texture(Texture0, TexCoords).g,
    	texture(Texture0, TexCoords + aberrated).b,
    	1.0
    );
}
