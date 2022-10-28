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
	
const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];
	
out vec2 TexCoords;
	
void main()
{
    //vec4 totalLocalPosition = vec4(0.0);
	//vec4 totalNormal = vec4(0.0);
	//for(int i = 0; i < MAX_BONE_INFLUENCE; i++)
	//{
	//	vec4 posePosition = finalBonesMatrices[l_boneIDs[i]] * vec4(l_position, 1.0);
	//	totalLocalPosition += posePosition * l_weights[i];
	//	vec4 worldNormal = finalBonesMatrices[l_boneIDs[i]] * vec4(l_normals, 0.0);
	//	totalNormal += worldNormal * l_weights[i];
	//}
	//gl_Position = PVMMatrix * totalLocalPosition;

	mat4 BoneTransform = finalBonesMatrices[l_boneIDs[0]] * l_weights[0];
    BoneTransform += finalBonesMatrices[l_boneIDs[1]] * l_weights[1];
    BoneTransform += finalBonesMatrices[l_boneIDs[2]] * l_weights[2];
    BoneTransform += finalBonesMatrices[l_boneIDs[3]] * l_weights[3];

    vec4 PosL = BoneTransform * vec4(l_position, 1.0);
    gl_Position = PVMatrix * PosL;

	TexCoords = l_texCoords;
}
