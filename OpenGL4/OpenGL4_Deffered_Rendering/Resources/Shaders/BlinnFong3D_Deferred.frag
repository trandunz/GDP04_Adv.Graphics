// Bachelor of Software Engineering 
// Media Design School 
// Auckland 
// New Zealand 
// (c) Media Design School
// File Name : BlinnFong3D.frag
// Author : William Inman
// Mail : william.inman@mds.ac.nz

#version 460 core

// Output to C++
layout (location = 0) out vec4 FragColor;

// Input from Previous Shader
in vec2 TexCoords;
in vec3 Position;
in vec3 Normals;

#define MAX_POINT_LIGHTS 100
#define MAX_DIRECTIONAL_LIGHTS 100
#define MAX_SPOT_LIGHTS 100

struct PointLight
{
    vec3 Position;
    vec4 Color;
    float SpecularStrength;

    float AttenuationLinear;
    float AttenuationExponent;
};

struct DirectionalLight
{
    vec3 Direction;
    vec4 Color;
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
uniform sampler2D gPosition;
uniform sampler2D gNormals;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;
uniform vec3 CameraPos;
uniform vec3 AmbientColor;
uniform float Shininess;
uniform float AmbientStrength;

uniform PointLight PointLights[MAX_POINT_LIGHTS];
uniform DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int NumberOfPointLights;
uniform int NumberOfDirectionalLights;

vec3 CalculateAmbientLight();
vec3 CalculatePointLight(PointLight _light);
vec3 CalculateDirectionalLight(DirectionalLight _light);

vec3 worldPos;
vec3 normal;
vec3 diffuse;
float spec;
vec3 ReverseViewDir;

// Main function that gets called per vertex fragment.
void main()
{
    worldPos = texture(gPosition, TexCoords).rgb;
    normal = texture(gNormals, TexCoords).rgb;
    diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    spec = texture(gAlbedoSpec, TexCoords).a;
    ReverseViewDir = normalize(CameraPos - worldPos);

    vec3 combinedLighting = CalculateAmbientLight();
    for (int i = 0; i < MAX_POINT_LIGHTS && i < NumberOfPointLights; i++)
    {
        combinedLighting += CalculatePointLight(PointLights[i]);
    }
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHTS && i < NumberOfDirectionalLights; i++)
    {
        combinedLighting += CalculateDirectionalLight(DirectionalLights[i]);
    }

    FragColor = vec4(combinedLighting,1.0f);
}

vec3 CalculateAmbientLight()
{
    return AmbientStrength * AmbientColor * diffuse;
}

vec3 CalculatePointLight(PointLight _light)
{
    vec3 lightDir = normalize(worldPos - _light.Position);

    float strength = max(dot(normal, -lightDir), 0.0f);
    vec3 diffuseLight = strength * diffuse * _light.Color.rgb;

    vec3 halfwayVector = normalize(-lightDir + ReverseViewDir);
    float specularReflectivity = pow(max(dot(normal, halfwayVector), 0.0f), Shininess);
    vec3 specularLight = _light.SpecularStrength * spec * specularReflectivity * _light.Color.rgb;

    float distance = length(_light.Position - worldPos);
    float attenuation = 1 + (_light.AttenuationLinear * distance) + (_light.AttenuationExponent * pow(distance, 2.0f));
    return (diffuseLight + specularLight) / attenuation;
}

vec3 CalculateDirectionalLight(DirectionalLight _light)
{
    float strength = max(dot(normal, -normalize(_light.Direction)), 0.0f);
    vec3 diffuseLight = strength * diffuse * _light.Color.rgb;

    vec3 halfwayVector = normalize(-normalize(_light.Direction) + ReverseViewDir);
    float specularReflectivity = pow(max(dot(normal, halfwayVector), 0.0f), Shininess);
    vec3 specularLight = _light.SpecularStrength * spec * specularReflectivity * _light.Color.rgb;

    return diffuseLight + specularLight;
}