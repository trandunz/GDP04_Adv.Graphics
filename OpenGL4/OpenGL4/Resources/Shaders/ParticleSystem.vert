// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : PositionOnly.vert
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Input locations from vertex buffer
layout (location = 0) in vec4 l_position;
layout (location = 1) in float l_rotation;

struct sTransform
{
    mat4 transform;
    vec3 rotation_axis;
    vec3 translation;
    vec3 scale;
    float rotation_value;
};

uniform mat4 PVMatrix;
uniform vec3 uTranslation;
uniform vec3 uScale;

out float LifeTime;

sTransform Transform; 

out VS_GS_VERTEX
{
	out vec3 AttribPosition;
	out mat4 PVMMatrix;
	out float LifeTime;
}vs_out;

mat4 Scale(mat4 _matrix, vec3 _v)
{
	mat4 Result;
	Result[0] = _matrix[0] * _matrix[0];
	Result[1] = _matrix[1] * _matrix[1];
	Result[2] = _matrix[2] * _matrix[2];
	Result[3] = _matrix[3];
	return Result;
}

mat4 Rotate(mat4 _matrix, float _angle, vec3 _v)
{
    float a = _angle;
	float c = cos(a);
	float s = sin(a);

	vec3 axis = normalize(_v);
	vec3 temp = (1.0f - c) * axis;

	mat4 Rotate;
	Rotate[0][0] = c + temp[0] * axis[0];
	Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
	Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

	Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
	Rotate[1][1] = c + temp[1] * axis[1];
	Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

	Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
	Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
	Rotate[2][2] = c + temp[2] * axis[2];

	mat4 Result;
	Result[0] = _matrix[0] * Rotate[0][0] + _matrix[1] * Rotate[0][1] + _matrix[2] * Rotate[0][2];
	Result[1] = _matrix[0] * Rotate[1][0] + _matrix[1] * Rotate[1][1] + _matrix[2] * Rotate[1][2];
	Result[2] = _matrix[0] * Rotate[2][0] + _matrix[1] * Rotate[2][1] + _matrix[2] * Rotate[2][2];
	Result[3] = _matrix[3];
	return Result;
}

mat4 Translate(mat4 _matrix, vec3 _v)
{
    mat4 Result = _matrix;
	Result[3] = _matrix[0] * _v[0] + _matrix[1] * _v[1] + _matrix[2] * _v[2] + _matrix[3];
	return Result;
}

void UpdateTransform()
{
	Transform.transform = Translate(Transform.transform, Transform.translation);
	if (Transform.rotation_axis.x > 0 ||
		Transform.rotation_axis.y > 0 ||
		Transform.rotation_axis.z > 0)
	{
		Transform.transform = Rotate(Transform.transform, Transform.rotation_value, Transform.rotation_axis);
	}
	Transform.transform = Scale(Transform.transform, Transform.scale);
}

// Main function that gets called per vertex.
void main()
{
	Transform.translation = uTranslation;
	Transform.scale = uScale;
	Transform.rotation_axis = vec3(0,0,1);
	Transform.rotation_value = l_rotation;
	UpdateTransform();

	vs_out.PVMMatrix = PVMatrix * Transform.transform;
	vs_out.AttribPosition = l_position.xyz;
	vs_out.LifeTime = l_position.w;
	gl_Position = PVMatrix * Transform.transform * vec4(l_position.xyz,1.0f);
	LifeTime = l_position.w;
}