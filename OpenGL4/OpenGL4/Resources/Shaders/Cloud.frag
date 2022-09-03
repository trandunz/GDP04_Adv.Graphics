// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Cloud.frag
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;
in vec4 WorldPos;

// Outside Variables Passed In As 'Uniforms'
uniform int TextureCount;
uniform sampler2D Texture0;
uniform bool Foggy;
uniform float FogStart;
uniform float FogDepth;
uniform vec3 CameraPos;
uniform vec4 FogColor;

// Available Helper function Forward Declerations
vec4 ColourFromTextureORWhite(vec2 _texCoords);

// Main function that gets called per vertex fragment.
void main()
{
    vec4 textureOutput = ColourFromTextureORWhite(TexCoords);
    if (textureOutput.r > 0.55f)
    {  
        textureOutput.a = 0.95f;
    }

    if (Foggy == true)
    {
        vec4 outColor = textureOutput;

        float d = distance(WorldPos.xyz, CameraPos);
        float contribution = (d - FogStart) / FogDepth;
        contribution = clamp(contribution, 0.0f, 1.0f);

        outColor = mix(textureOutput, FogColor, contribution);
        FragColor = vec4(outColor.rgb, textureOutput.a);

    }
    else
    {
        FragColor = textureOutput;
    }
}

// Checks for number a texture and returns the colour output accordingly.
vec4 ColourFromTextureORWhite(vec2 _texCoords)
{
    vec4 outputColour;
    switch(TextureCount)
    {
        case 0:
        {
            outputColour = vec4(1.0f,1.0f,1.0f,1.0f);
            break;
        }
        default:
        {
            outputColour = texture(Texture0,_texCoords);
            break;
        }
    }
    return outputColour;
}
