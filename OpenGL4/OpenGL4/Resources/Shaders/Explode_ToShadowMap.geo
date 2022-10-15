#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_GS_VERTEX
{
	in vec4 Position;
	in vec4 AttribPosition;
	in mat4 PVMMatrix;
	in mat4 LightVPMatrix;
	in mat4 Model;
}gs_in[];

out vec2 TexCoords;
out vec3 Position;
out vec3 Normals;
out vec4 FragPosLightSpace;

uniform float ElapsedTime;
uniform float Magnitude;

vec4 Explode(vec4 _position, vec3 _normal);
vec3 GetNormal();

void main()
{
	vec3 normal = GetNormal();

    gl_Position = gs_in[0].LightVPMatrix * gs_in[0].Model * Explode(gs_in[0].AttribPosition, normal);
    EmitVertex();
    gl_Position = gs_in[1].LightVPMatrix * gs_in[1].Model * Explode(gs_in[1].AttribPosition, normal);
    EmitVertex();
    gl_Position = gs_in[2].LightVPMatrix * gs_in[2].Model * Explode(gs_in[2].AttribPosition, normal);
    EmitVertex();

	EndPrimitive();
}

vec4 Explode(vec4 _position, vec3 _normal)
{
    vec3 direction = _normal * ((sin(ElapsedTime) + 1.0) / 2.0) * Magnitude; 
    return _position + vec4(direction, 0.0);
} 

vec3 GetNormal()
{
   vec3 a = vec3(gs_in[0].AttribPosition) - vec3(gs_in[1].AttribPosition);
   vec3 b = vec3(gs_in[2].AttribPosition) - vec3(gs_in[1].AttribPosition);
   return normalize(cross(a, b));
}  