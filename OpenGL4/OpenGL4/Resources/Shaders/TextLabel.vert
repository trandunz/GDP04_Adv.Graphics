// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : TextLabel.vert
// Description : Generic vertex shader for handling TextLabels And Scrolling TextLabels
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec4 l_vertex;

// Output data for next Shader
out vec2 TexCoords;
out vec2 Position;
out float LeftClipPass;
out float RightClipPass;

// Outside Variables Passed In As 'Uniforms'
uniform mat4 PMatrix;
uniform float LeftClip;
uniform float RightClip;
uniform float ElapsedTime;
uniform float ScrollSpeed;
uniform bool IsScrollingRight;
uniform bool IsScrolling;

// Main function that gets called per vertex.
void main()
{
	LeftClipPass = LeftClip;
	RightClipPass = RightClip;

	TexCoords = l_vertex.zw;

	Position = l_vertex.xy;
	if (IsScrolling)
	{
		if (IsScrollingRight)
		{
			Position.x = mod((l_vertex.x + ElapsedTime * ScrollSpeed) - LeftClip, RightClip - LeftClip) + LeftClip;
		}
		else
		{
			Position.x = mod((l_vertex.x - ElapsedTime * ScrollSpeed) - LeftClip, RightClip - LeftClip) + LeftClip;
		}
	}

	gl_Position = PMatrix * vec4(Position, 0.0f, 1.0f);
}