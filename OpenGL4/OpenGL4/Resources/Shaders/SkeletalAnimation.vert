// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : SingleTexture.vert
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec3 l_position;
layout (location = 1) in vec2 l_texCoords;
layout (location = 2) in vec3 l_normals;
layout (location = 3) in ivec4 l_boneIDs;
layout (location = 4) in vec4 l_weights;

uniform mat4 PVMatrix;
uniform mat4 ModelMatrix;
	
const int MAX_JOINTS = 100;
const int MAX_WEIGHTS = 16;

uniform mat4 jointTransforms[MAX_JOINTS];
	
out vec2 TexCoords;
out vec3 Normals;
out vec3 Position;
	
void main()
{
	TexCoords = l_texCoords;

    vec4 totalLocalPosition = vec4(0.0f);
	vec4 totalNormal = vec4(0.0f);
	for(int i = 0; i < MAX_WEIGHTS; i++)
	{
		vec4 posePosition = jointTransforms[l_boneIDs[i]] * vec4(l_position, 1.0);
		totalLocalPosition += posePosition * l_weights[i];
		vec4 worldNormal = normalize(transpose(inverse(jointTransforms[l_boneIDs[i]])) * vec4(l_normals, 0.0));
		totalNormal += worldNormal * l_weights[i];
	}

	Normals = vec3(totalNormal);
	Position = vec3(ModelMatrix * vec4(totalLocalPosition.xyz, 1.0f));

	gl_Position = PVMatrix * ModelMatrix * totalLocalPosition;
}
