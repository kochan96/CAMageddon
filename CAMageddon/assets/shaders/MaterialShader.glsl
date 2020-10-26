//Material Shader


#type vertex
#version 440 core
#extension GL_EXT_gpu_shader4 : enable

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

uniform sampler2D u_HeightMap;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TexCoord;

vec3 CalculateNormalFromHeightMap()
{
    const vec2 size = vec2(2.0,0.0);
    const ivec3 off = ivec3(-1,0,1);

    float heightTop = textureOffset(u_HeightMap, a_TexCoord, ivec2(0, 1)).r;
    float heightBottom = textureOffset(u_HeightMap, a_TexCoord, ivec2(0, -1)).r;
    float heightRight = textureOffset(u_HeightMap, a_TexCoord, ivec2(1, 0)).r;
    float heightLeft = textureOffset(u_HeightMap, a_TexCoord, ivec2(-1, 0)).r;

    vec3 vx = vec3(2.0f,0.0f,(heightRight-heightLeft));
    vec3 vy = vec3(0.0f,2.0f,(heightTop-heightBottom));

    return normalize(cross(vx,vy));
    
}

void main()
{
    float height = texture(u_HeightMap, a_TexCoord).r;
    vec3 position = a_Position + vec3(0.0f, 0.0f, height);
    gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(position, 1.0f);
 
    vec3 normal = CalculateNormalFromHeightMap();

    v_FragPos = vec3(u_ModelMatrix * vec4(position, 1.0f));
    //v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * a_Normal;
    v_TexCoord = a_TexCoord;

    v_Normal = normal;
}


#type fragment
#version 440 core

layout(location = 0) out vec4 color;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Attenuation
    bool attenuationEnabled;
    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // Attenuation
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cutOff;
    float outerCutOff;

    // Attenuation
    bool attenuationEnabled;
    float constant;
    float linear;
    float quadratic;
};

#define MAX_POINT_LIGHT_COUNT 10
#define MAX_DIR_LIGHT_COUNT 10
#define MAX_SPOT_LIGHT_COUNT 10

uniform int u_PointlightCount;
uniform int u_DirlightCount;
uniform int u_SpotlightCount;

uniform PointLight u_PointLights[MAX_POINT_LIGHT_COUNT];
uniform DirLight u_DirLights[MAX_DIR_LIGHT_COUNT];
uniform SpotLight u_SpotLights[MAX_SPOT_LIGHT_COUNT];

uniform Material u_Material;

uniform vec3 u_ViewPosition;

uniform sampler2D u_HeightMap;

in vec3 v_Normal;
in vec3 v_FragPos;
in vec2 v_TexCoord;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 ComputeAmbient(vec3 ambientLight);
vec4 ComputeDiffuse(vec3 normal, vec3 lightDir, vec3 diffuseLight);
vec3 ComputeSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 specularLight);

float ComputeAttenuation(
    vec3 lightPosition, 
    vec3 fragPosition,
    float constant, 
    float linear, 
    float quadratic);

void main()
{
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPosition - v_FragPos);

    vec3 result = vec3(0.0f,0.0f,0.0f);
    for(int i = 0; i< u_DirlightCount; i++)
    {
        result += CalcDirLight(u_DirLights[i], norm, viewDir);
    }

    for(int i=0;i<u_PointlightCount;i++)
    {
        result += CalcPointLight(u_PointLights[i], norm, v_FragPos, viewDir);
    }

    for(int i = 0; i < u_SpotlightCount; i++)
    {
        result += CalcSpotLight(u_SpotLights[i], norm, v_FragPos, viewDir);
    }

    color = vec4(result, 1.0f);

}

vec3 ComputeAmbient(vec3 ambientLight)
{
    return ambientLight * texture(u_Material.diffuse, v_TexCoord).rgb;
}

vec4 ComputeDiffuse(vec3 normal, vec3 lightDir, vec3 diffuseLight)
{
    float diffSpec = dot(normal, lightDir);
    float diff = max(diffSpec, 0.0f);
    vec3 diffuse = diffuseLight * diff * texture(u_Material.diffuse, v_TexCoord).rgb;

    return vec4(diffuse, diffSpec);
}

vec3 ComputeSpecular(vec3 normal, vec3 lightDir, vec3 viewDir, vec3 specularLight)
{
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);

    return specularLight * spec * texture(u_Material.specular, v_TexCoord).rgb;
}

float ComputeAttenuation(
    vec3 lightPosition, 
    vec3 fragPosition,
    float constant, 
    float linear, 
    float quadratic)
{
    float distance    = length(lightPosition - fragPosition);
    float attenuation = 1.0f / 
    ((constant) + (linear * distance) +
    (quadratic * (distance* distance)));

    return attenuation;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);

    vec3 ambient = ComputeAmbient(light.ambient);
    vec4 diffuse = ComputeDiffuse(normal, lightDir, light.diffuse);
    vec3 specular = vec3(0.0f);
    if(diffuse.w > 0.0f)
        specular = ComputeSpecular(normal, lightDir, viewDir, light.specular);

    //Attenuation
    if(light.attenuationEnabled)
    {
        float attenuation = ComputeAttenuation(light.position, fragPos, light.constant, light.linear, light.quadratic);
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
    }

    return (ambient + diffuse.xyz + specular);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    
    vec3 ambient = ComputeAmbient(light.ambient);
    vec4 diffuse = ComputeDiffuse(normal, lightDir, light.diffuse);
    vec3 specular = vec3(0.0f);
    if(diffuse.w > 0.0f)
        specular = ComputeSpecular(normal, lightDir, viewDir, light.specular);

    return (ambient + diffuse.xyz + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    //TODO
    return vec3(0.0f);
}