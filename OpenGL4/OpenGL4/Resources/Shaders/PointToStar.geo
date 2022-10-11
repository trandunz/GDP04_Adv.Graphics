#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 30) out;

out vec2 TexCoords;
out vec3 Normals;
out vec3 Position;
out vec4 FragPosLightSpace;

in VS_GS_VERTEX
{
	in vec4 Position;
	in vec4 AttribPosition;
	in mat4 PVMMatrix;
	in mat4 LightVPMatrix;
	in mat4 Model;
}gs_in[];

void CreateOutputNormalsAndPosition();

void main()
{
	// Bottom Left Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f, 0.5f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.2f, -0.1f, 0.0f, 0.0f);
	TexCoords = vec2(0.3f,0.4f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(-0.2f, -0.1f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f, 0.3f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	EmitVertex();


	// Bottom Left Point
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.2f, -0.1f, 0.0f, 0.0f);
	TexCoords = vec2(0.3f,0.4f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(-0.2f, -0.1f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.3f, -0.5f, 0.0f, 0.0f);
	TexCoords = vec2(0.2f,0.0f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(-0.3f, -0.5f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f,0.3f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	EmitVertex();

	// Bottom Right Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f); 
	TexCoords = vec2(0.5f,0.5f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f,0.3f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.2f, -0.1f, 0.0f, 0.0f);
	TexCoords = vec2(0.7f,0.4f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.2f, -0.1f, 0.0f, 0.0f);
	EmitVertex();

	// Bottom Right Point
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f,0.3f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.0f, -0.2f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.3f, -0.5f, 0.0f, 0.0f);
	TexCoords = vec2(0.8f,0.0f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.3f, -0.5f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.2f, -0.1f, 0.0f, 0.0f);
	TexCoords = vec2(0.7f,0.4f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.2f, -0.1f, 0.0f, 0.0f);
	EmitVertex();

	// Top Right Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f); 
	TexCoords = vec2(0.5f,0.5f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.2f, -0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.7f,0.4f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.2f, -0.1f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.6f,0.6f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.1f, +0.1f, 0.0f, 0.0f);
	EmitVertex();

	// Top Right Point
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.2f, -0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.7f,0.4f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.2f, -0.1f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.5f, 0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(1.0f,0.6f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.5f, 0.1f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.1f, +0.1f, 0.0f, 0.0f);
	TexCoords = vec2(0.6f,0.6f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.1f, +0.1f, 0.0f, 0.0f);
	EmitVertex();

	// Top Middle Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f); 
	TexCoords = vec2(0.5f,0.5f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.6f,0.6f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.1f, +0.1f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.4f,0.6f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(-0.1f, +0.1f, 0.0f, 0.0f);
	EmitVertex();


	// Top Middle Point
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.6f,0.6f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.1f, +0.1f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.5f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f,1.0f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.0f, 0.5f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.4f,0.6f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(-0.1f, +0.1f, 0.0f, 0.0f);
	EmitVertex();

	// Top left Centre
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	TexCoords = vec2(0.5f,0.5f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(0.0f, 0.0f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.4f,0.6f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(-0.1f, +0.1f, 0.0f, 0.0f);
	EmitVertex();
	
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.2f, -0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.3f,0.4f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(-0.2f, -0.1f, 0.0f, 0.0f);
	EmitVertex();


	// Top left Point
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.1f, +0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.4f,0.6f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(-0.1f, +0.1f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.5f, 0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.0f,0.6f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(-0.5f, 0.1f, 0.0f, 0.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + gs_in[0].PVMMatrix * vec4(-0.2f, -0.1f, 0.0f, 0.0f); 
	TexCoords = vec2(0.3f,0.4f);
	CreateOutputNormalsAndPosition();
	FragPosLightSpace = gs_in[0].AttribPosition + gs_in[0].LightVPMatrix * gs_in[0].Model * vec4(-0.2f, -0.1f, 0.0f, 0.0f);
	EmitVertex();

	EndPrimitive();
}

void CreateOutputNormalsAndPosition()
{
	Normals = normalize(mat3(transpose(inverse(gs_in[0].Model))) * vec3(0.0f,0.0f,-1.0f));
	Position = vec3(gl_Position);
}
