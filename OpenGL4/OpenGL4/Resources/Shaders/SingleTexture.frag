// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : SingleTexture.frag
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;

// Outside Variables Passed In As 'Uniforms'
uniform int TextureCount;
uniform sampler2D Texture0;

// Available Helper function Forward Declerations
vec4 ColourFromTextureORWhite(vec2 _texCoords);

// Main function that gets called per vertex fragment.
void main()
{
    FragColor = ColourFromTextureORWhite(TexCoords);
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
