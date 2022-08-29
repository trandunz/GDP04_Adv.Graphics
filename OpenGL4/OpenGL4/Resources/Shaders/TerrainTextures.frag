// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : SingleTexture.frag
// Description : Generic fragment shader for handling single textured Meshes
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;
in vec3 Position;
in vec3 Normals;
in float Height;

#define MAX_POINT_LIGHTS 4
#define MAX_DIRECTIONAL_LIGHTS 4
#define MAX_SPOT_LIGHTS 4

struct PointLight
{
    vec3 Position;
    vec3 Color;
    float SpecularStrength;

    float AttenuationLinear;
    float AttenuationExponent;
};

struct DirectionalLight
{
    vec3 Direction;
    vec3 Color;
    float SpecularStrength;
};

struct SpotLight
{
    vec3 Position;
    vec3 Color;
    float SpecularStrength;
    vec3 Direction;

    float AttenuationLinear;
    float AttenuationExponent;

    float Cutoff;
    float OuterCutoff;
};

// Outside Variables Passed In As 'Uniforms'
uniform int TextureCount;
uniform sampler2D Texture0;
uniform sampler2D Texture1;
uniform sampler2D Texture2;
uniform sampler2D Texture3;

uniform float Texture0Height;
uniform float Texture1Height;
uniform float Texture2Height;
uniform float Texture3Height;

uniform vec3 CameraPos;
uniform vec3 AmbientColor;
uniform float Shininess;
uniform float AmbientStrength;

uniform int NumberOfPointLights;
uniform int NumberOfDirectionalLights;
uniform int NumberOfSpotLights;

uniform PointLight PointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform SpotLight SpotLights[MAX_SPOT_LIGHTS];

uniform bool bRimLighting;
uniform float RimExponent;
uniform vec3 RimColor;

uniform bool Foggy;
uniform float FogStart;
uniform float FogDepth;
uniform vec4 FogColor;

// Available Helper function Forward Declerations
vec4 ColourFromTextureORWhite(vec2 _texCoords);
vec3 CalculateAmbientLight();

vec3 CalculatePointLight(PointLight _pointLight);
vec3 CalculateDirectionalLight(DirectionalLight _directionalLight);
vec3 CalculateSpotLight(SpotLight _spotLight);
vec3 CalculateRimLight();

vec3 ReverseViewDir;

void main()
{
    ReverseViewDir = normalize(CameraPos - Position);

    vec3 combinedLighting = CalculateAmbientLight();
    for (int i = 0; i < MAX_POINT_LIGHTS && i < NumberOfPointLights; i++)
    {
        combinedLighting += CalculatePointLight(PointLights[i]);
    }
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS && i < NumberOfDirectionalLights; i++)
    {
        combinedLighting += CalculateDirectionalLight(DirectionalLights[i]);
    }
    for (int i = 0; i < MAX_SPOT_LIGHTS && i < NumberOfSpotLights; i++)
    {
        combinedLighting += CalculateSpotLight(SpotLights[i]);
    }
    if (bRimLighting)
    {
        combinedLighting += CalculateRimLight();
    }

    if (Foggy == true)
    {
        float d = distance(Position, CameraPos);
        float contribution = (d - FogStart) / FogDepth;
        contribution = clamp(contribution, 0.0f, 1.0f);

        if (Height >= Texture3Height && TextureCount >= 4)
        {
            FragColor =  mix(vec4(combinedLighting,1.0f) * texture(Texture3,TexCoords), FogColor, contribution);
        }
        else if (Height >= Texture2Height && TextureCount >= 3)
        {
            FragColor =  mix(vec4(combinedLighting,1.0f) * texture(Texture2,TexCoords), FogColor, contribution);
        }
        else if (Height >= Texture1Height && TextureCount >= 4)
        {
            FragColor =  mix(vec4(combinedLighting,1.0f) * texture(Texture1,TexCoords), FogColor, contribution);
        }
        else if (Height >= Texture0Height && TextureCount >= 1)
        {
            FragColor =  mix(vec4(combinedLighting,1.0f) * texture(Texture0,TexCoords), FogColor, contribution);
        }
        else if (TextureCount >= 1)
        {
            FragColor =  mix(vec4(combinedLighting,1.0f) * texture(Texture0,TexCoords), FogColor, contribution);
        }
        else
        {   
            FragColor = mix(vec4(1.0f,1.0f,1.0f,1.0f), FogColor, contribution);
        }
    }
    else
    {
        if (Height >= Texture3Height && TextureCount >= 4)
        {
            FragColor = vec4(combinedLighting,1.0f) * texture(Texture3,TexCoords);
        }
        else if (Height >= Texture2Height && TextureCount >= 3)
        {
            FragColor = vec4(combinedLighting,1.0f) * texture(Texture2,TexCoords);
        }
        else if (Height >= Texture1Height && TextureCount >= 4)
        {
            FragColor = vec4(combinedLighting,1.0f) * texture(Texture1,TexCoords);
        }
        else if (Height >= Texture0Height && TextureCount >= 1)
        {
            FragColor = vec4(combinedLighting,1.0f) * texture(Texture0,TexCoords);
        }
        else if (TextureCount >= 1)
        {
            FragColor = vec4(combinedLighting,1.0f) * texture(Texture0,TexCoords);
        }
        else
        {   
            FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
        }
    }

    
}

// Calculates Ambient Lighting
vec3 CalculateAmbientLight()
{
    return AmbientStrength * AmbientColor;
}

// Calculates Rim Lighting
vec3 CalculateRimLight()
{
    float rimFactor = 1.0f - dot(Normals, ReverseViewDir);
    rimFactor = smoothstep(0.0f, 1.0f, rimFactor);
    rimFactor = pow(rimFactor, RimExponent);
    return rimFactor * RimColor;
}

// Calculates A Point Lights Contribution
vec3 CalculatePointLight(PointLight _pointLight)
{
    vec3 lightDir = normalize(Position - _pointLight.Position);

    float strength = max(dot(Normals, -lightDir), 0.0f);
    vec3 diffuseLight = strength * _pointLight.Color;

    vec3 halfwayVector = normalize(-lightDir + ReverseViewDir);
    float specularReflectivity = pow(max(dot(Normals, halfwayVector), 0.0f), Shininess);
    vec3 specularLight = _pointLight.SpecularStrength * specularReflectivity * _pointLight.Color;

    float distance = length(_pointLight.Position - Position);
    float attenuation = 1 + (_pointLight.AttenuationLinear * distance) + (_pointLight.AttenuationExponent * pow(distance, 2.0f));
    return (diffuseLight + specularLight) / attenuation;
}

// Calculates A Directional Lights Contribution
vec3 CalculateDirectionalLight(DirectionalLight _directionalLight)
{
    float strength = max(dot(Normals, -normalize(_directionalLight.Direction)), 0.0f);
    vec3 diffuseLight = strength * _directionalLight.Color;

    vec3 halfwayVector = normalize(-normalize(_directionalLight.Direction) + ReverseViewDir);
    float specularReflectivity = pow(max(dot(Normals, halfwayVector), 0.0f), Shininess);
    vec3 specularLight = _directionalLight.SpecularStrength * specularReflectivity * _directionalLight.Color;

    return diffuseLight + specularLight;
}

// Calculates A Spot Lights Contribution
vec3 CalculateSpotLight(SpotLight _spotLight)
{
    vec3 lightDir = normalize(Position - _spotLight.Position);

    float strength = max(dot(Normals, -lightDir), 0.0f);
    vec3 diffuseLight = strength * _spotLight.Color;

    vec3 halfwayVector = normalize(-lightDir + ReverseViewDir);
    float specularReflectivity = pow(max(dot(Normals, halfwayVector), 0.0f), Shininess);
    vec3 specularLight = _spotLight.SpecularStrength * specularReflectivity * _spotLight.Color;

    float distance = length(_spotLight.Position - Position);
    float attenuation = 1 + (_spotLight.AttenuationLinear * distance) + (_spotLight.AttenuationExponent * pow(distance, 2.0f)); 

    float angle = dot(lightDir, normalize(_spotLight.Direction));
    float intensity = clamp((angle - _spotLight.OuterCutoff) / (_spotLight.Cutoff - _spotLight.OuterCutoff), 0.0f, 1.0f);
    return ((diffuseLight + specularLight) * intensity) / attenuation;
}