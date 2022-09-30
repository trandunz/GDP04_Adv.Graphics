#version 460 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_GS_VERTEX
{
	in vec4 Position;
	in mat4 PVMMatrix;
	in vec3 Normals;
	in vec2 TexCoords;
}gs_in[];

out vec2 TexCoords;

uniform float ElapsedTime;
uniform float Magnitude;

vec4 Explode(vec4 _position, vec3 _normal);
vec3 GetNormal();

void main()
{
	vec3 normal = GetNormal();

    gl_Position = Explode(gl_in[0].gl_Position, normal);
    TexCoords = gs_in[0].TexCoords;
    EmitVertex();
    gl_Position = Explode(gl_in[1].gl_Position, normal);
    TexCoords = gs_in[1].TexCoords;
    EmitVertex();
    gl_Position = Explode(gl_in[2].gl_Position, normal);
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
