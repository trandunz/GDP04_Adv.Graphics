#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 30) out;

out vec2 TexCoords;

in VS_GS_VERTEX
{
	in vec4 Position;
	in mat4 PVMMatrix;
}gs_in[];

void main()
{
	// Bottom Left Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f, 0.5f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f, 0.3f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.2f, -0.1f, 0.0f, 0.0f);
	TexCoords = vec2(0.3f,0.4f);
	EmitVertex();

	// Bottom Left Point
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f,0.3f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.2f, -0.1f, 0.0f, 0.0f);
	TexCoords = vec2(0.3f,0.4f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.3f, -0.5f, 0.0f, 0.0f);
	TexCoords = vec2(0.2f,0.0f);
	EmitVertex();

	// Bottom Right Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f); 
	TexCoords = vec2(0.5f,0.5f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f,0.3f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.2f, -0.1f, 0.0f, 0.0f);
	TexCoords = vec2(0.7f,0.4f);
	EmitVertex();

	// Bottom Right Point
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f,0.3f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.2f, -0.1f, 0.0f, 0.0f);
	TexCoords = vec2(0.7f,0.4f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.3f, -0.5f, 0.0f, 0.0f);
	TexCoords = vec2(0.8f,0.0f);
	EmitVertex();

	// Top Right Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f); 
	TexCoords = vec2(0.5f,0.5f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.2f, -0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.7f,0.4f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.6f,0.6f);
	EmitVertex();

	// Top Right Point
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.2f, -0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.7f,0.4f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.1f, +0.1f, 0.0f, 0.0f);
	TexCoords = vec2(0.6f,0.6f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.5f, 0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(1.0f,0.6f);
	EmitVertex();

	// Top Middle Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f,0.5f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.2f, -0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.3f,0.4f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.4f,0.6f);
	EmitVertex();

	// Top Middle Point
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.125f, 0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.5f - 0.125f,0.6f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.125f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.5f + 0.125f,0.6f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.5f, 0.0f, 0.0f); 
	TexCoords = vec2(0.5f,1.0f);
	EmitVertex();

	// Top Left Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f); 
	TexCoords = vec2(0.5f,0.5f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.4f,0.6f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.6f,0.6f);
	EmitVertex();

	// Top Left Point
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.2f, -0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.3f,0.4f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.4f,0.6f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.5f, 0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.0f,0.6f);
	EmitVertex();

	EndPrimitive();
}
