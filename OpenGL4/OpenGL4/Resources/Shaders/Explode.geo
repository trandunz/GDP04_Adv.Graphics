// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : Explode.geo
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_GS_VERTEX
{
	in vec4 Position;
    in vec4 LocalPos;
	in mat4 PVMMatrix;
	in vec3 Normals;
	in vec2 TexCoords;
}gs_in[];

out vec2 TexCoords;
out vec3 Position;
out vec3 Normals;
out vec4 FragPosLightSpace;

uniform float ElapsedTime;
uniform float Magnitude;
uniform mat4 ModelMatrix;
uniform mat4 LightVPMatrix;

// returns the position of the vertex exploded along the normal
vec4 Explode(vec4 _position, vec3 _normal);
// interpolates the vertex positions and returns the normal
vec3 GetNormal();

void main()
{
	vec3 normal = GetNormal();

    // set exploded position
    gl_Position = Explode(gl_in[0].gl_Position, normal);
    // position for lighting
    Position = vec3(ModelMatrix * gs_in[0].LocalPos);
    // normals for lighting
    Normals = gs_in[0].Normals;
    // frag pos in light space
    FragPosLightSpace = LightVPMatrix * vec4(Position, 1.0f);
    // tex coords
    TexCoords = gs_in[0].TexCoords;
    EmitVertex();

    //
    gl_Position = Explode(gl_in[1].gl_Position, normal);
    //
    Position = vec3(ModelMatrix * gs_in[1].LocalPos);
    //
    Normals = gs_in[1].Normals;
    FragPosLightSpace = LightVPMatrix * vec4(Position, 1.0f);
    //
    TexCoords = gs_in[1].TexCoords;
    EmitVertex();

    //
    gl_Position = Explode(gl_in[2].gl_Position, normal);
    //
    Position = vec3(ModelMatrix * gs_in[2].LocalPos);
    //
    Normals = gs_in[2].Normals;
    //
    FragPosLightSpace = LightVPMatrix * vec4(Position, 1.0f);
    //
    TexCoords = gs_in[2].TexCoords;
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
   vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
   vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
   return normalize(cross(a, b));
}  
