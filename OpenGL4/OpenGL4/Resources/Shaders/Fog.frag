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
    if (Foggy == true)
    {
        float d = distance(WorldPos.xyz, CameraPos);
        float contribution = (d - FogStart) / FogDepth;
        contribution = clamp(contribution, 0.0f, 1.0f);

        FragColor = mix(ColourFromTextureORWhite(TexCoords), FogColor, contribution);
    }
    else
    {
        FragColor = ColourFromTextureORWhite(TexCoords);
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
