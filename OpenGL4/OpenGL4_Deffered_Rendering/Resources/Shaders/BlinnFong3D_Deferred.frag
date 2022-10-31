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
uniform sampler2D gPosition;
uniform sampler2D gNormals;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;

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

// Available Helper function Forward Declerations
vec4 ColourFromTextureORWhite(vec2 _texCoords);
vec3 CalculateAmbientLight(vec3 _diffuse);

vec3 CalculatePointLight(PointLight _pointLight, vec3 _position, vec3 _normals, float _spec, vec3 _diffuse);
vec3 CalculateDirectionalLight(DirectionalLight _directionalLight, vec3 _position, vec3 _normals, float _spec);
vec3 CalculateSpotLight(SpotLight _spotLight, vec3 _position, vec3 _normals, float _spec);
vec3 CalculateRimLight();

vec3 ReverseViewDir;

// Main function that gets called per vertex fragment.
void main()
{
    vec3 worldPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormals, TexCoords).rgb;
    vec3 diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float spec = texture(gAlbedoSpec, TexCoords).a;
    ReverseViewDir = normalize(CameraPos - worldPos);

    vec3 combinedLighting = CalculateAmbientLight(diffuse);
    for (int i = 0; i < MAX_POINT_LIGHTS && i < NumberOfPointLights; i++)
    {
        combinedLighting += CalculatePointLight(PointLights[i], worldPos, normal, spec, diffuse);
    }

    FragColor = vec4(combinedLighting,1.0f);
}

// Calculates Ambient Lighting
vec3 CalculateAmbientLight(vec3 _diffuse)
{
    return _diffuse * 0.1f;
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
vec3 CalculatePointLight(PointLight _pointLight, vec3 _position, vec3 _normals, float _spec, vec3 _diffuse)
{
    vec3 lightDir = normalize(_position - _pointLight.Position);
    return max(dot(_normals, -lightDir), 0.0f) * _diffuse * _pointLight.Color;
}

// Calculates A Directional Lights Contribution
vec3 CalculateDirectionalLight(DirectionalLight _directionalLight, vec3 _position, vec3 _normals, float _spec)
{
    float strength = max(dot(_normals, -normalize(_directionalLight.Direction)), 0.0f);
    vec3 diffuseLight = strength * _directionalLight.Color;

    vec3 halfwayVector = normalize(-normalize(_directionalLight.Direction) + ReverseViewDir);
    float specularReflectivity = _spec;
    vec3 specularLight = _directionalLight.SpecularStrength * specularReflectivity * _directionalLight.Color;

    return diffuseLight + specularLight;
}

// Calculates A Spot Lights Contribution
vec3 CalculateSpotLight(SpotLight _spotLight, vec3 _position, vec3 _normals, float _spec)
{
    vec3 lightDir = normalize(_position - _spotLight.Position);

    float strength = max(dot(_normals, -lightDir), 0.0f);
    vec3 diffuseLight = strength * _spotLight.Color;

    vec3 halfwayVector = normalize(-lightDir + ReverseViewDir);
    float specularReflectivity = _spec;
    vec3 specularLight = _spotLight.SpecularStrength * specularReflectivity * _spotLight.Color;

    float distance = length(_spotLight.Position - _position);
    float attenuation = 1 + (_spotLight.AttenuationLinear * distance) + (_spotLight.AttenuationExponent * pow(distance, 2.0f)); 

    float angle = dot(lightDir, normalize(_spotLight.Direction));
    float intensity = clamp((angle - _spotLight.OuterCutoff) / (_spotLight.Cutoff - _spotLight.OuterCutoff), 0.0f, 1.0f);
    return ((diffuseLight + specularLight) * intensity) / attenuation;
}