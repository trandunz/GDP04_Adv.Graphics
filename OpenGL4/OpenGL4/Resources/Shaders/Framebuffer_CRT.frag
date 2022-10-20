// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Framebuffer_CRT.frag
// Author : William Inman
// Mail : william.inman@mds.ac.nz
// Credit to: Hoyong , https://www.shadertoy.com/view/ttffzs

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;

// Outside Variables Passed In As 'Uniforms'
uniform sampler2D Texture0;
uniform vec2 WindowSize;
uniform float ElapsedTime;

// Main function that gets called per vertex fragment.
void main()
{
    vec2 uv = TexCoords;
    uv = (uv-0.5f)*2.0f;
    uv *= 1.1f;
    
    uv.x *= 1. + pow(abs(uv.y/2.0f),3.0f);
    uv.y *= 1. + pow(abs(uv.x/2.0f),3.0f);
    uv = (uv + 1.0f)*0.5f;
    
    vec4 tex = vec4( 
        texture(Texture0,uv+0.004f).r,
        texture(Texture0,uv).g,
        texture(Texture0,uv-0.004f).b, 1.0f
    );
    //little thin lines                         //big thick lines
    float lines = 0.01*cos(uv.y*WindowSize.y) + 0.1f*sin(uv.y*20.0f + ElapsedTime);
    tex.rgb += lines;
    
    //smooth ver
    tex *= smoothstep(uv.x,uv.x+0.01f,1.0f)*smoothstep(uv.y,uv.y+0.01f,1.0f)*smoothstep(-0.01f,0.0f,uv.x)*smoothstep(-0.01f,0.0f,uv.y);
    
    float avg = (tex.r+tex.g+tex.b)/3.0f;
    FragColor = tex + pow(avg,3.0f);
}
