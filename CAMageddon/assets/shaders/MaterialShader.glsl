//Material Shader

#type vertex
#version 440 core

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

    vec3 vx = normalize(vec3(2.0f,0.0f,heightRight-heightLeft));
    vec3 vy = normalize(vec3(0.0f,2.0f,heightTop-heightBottom));

    return cross(vx,vy);
}

void main()
{
    float height = texture(u_HeightMap, a_TexCoord).r;
    vec3 position = a_Position + vec3(0.0f, 0.0f, height);
    gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(position, 1.0f);
 
    vec3 normal = CalculateNormalFromHeightMap();

    v_FragPos = vec3(u_ModelMatrix * vec4(position, 1.0f));
    v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * normal;
    v_TexCoord = a_TexCoord;
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
    //float constant;
    //float linear;
    //float quadratic;
};

#define MAX_LIGHT_COUNT 10
uniform int u_PointlightCount;
uniform PointLight u_PointLights[MAX_LIGHT_COUNT];

uniform Material u_Material;

uniform vec3 u_ViewPosition;

uniform sampler2D u_HeightMap;

in vec3 v_Normal;
in vec3 v_FragPos;
in vec2 v_TexCoord;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(v_Normal);
    vec3 viewDir = normalize(u_ViewPosition - v_FragPos);

    vec3 result = vec3(0.0f,0.0f,0.0f);
    for(int i=0;i<u_PointlightCount;i++)
    {
        result += CalcPointLight(u_PointLights[i], norm, v_FragPos, viewDir);
    }

    color = vec4(result, 1.0f);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    //diffuse shading
    float diff = dot(normal, lightDir);
    //specular shading
    float spec = 0.0f;
    if(diff>0.0f)
    {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0f), u_Material.shininess);
    }

    diff = max(diff, 0.0f);

    //Attenuation (maybe TODO)

    vec3 ambient = light.ambient * texture(u_Material.diffuse, v_TexCoord).rgb;
    vec3 diffuse = light.diffuse * diff * texture(u_Material.diffuse, v_TexCoord).rgb;
    vec3 specular = light.specular * spec * texture(u_Material.specular, v_TexCoord).rgb;

    return (ambient + diffuse + specular);
}