// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : TextLabel.frag
// Description : Generic fragment shader for handling TextLabels And Scrolling TextLabels
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;
in vec2 Position;
in float LeftClipPass;
in float RightClipPass;

// Outside Variables Passed In As 'Uniforms'
uniform sampler2D Texture;
uniform vec4 Colour;

// Main function that gets called per vertex fragment.
void main()
{
    float alpha = texture2D(Texture, TexCoords).r;
    if (Position.x <= (LeftClipPass + 50) || Position.x >= (RightClipPass - 50))
    { 
        alpha = 0.0f;
    }
    FragColor = vec4(Colour.xyz, alpha);
} 