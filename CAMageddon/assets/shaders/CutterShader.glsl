//CutterShader

#type vertex
#version 440 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec3 a_Normal;
layout (location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TexCoord;


void main()
{
    vec3 position = a_Position;
    gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(position, 1.0f);
    
    v_FragPos = vec3(u_ModelMatrix * vec4(position, 1.0f));
    v_Normal = mat3(transpose(inverse(u_ModelMatrix))) * a_Normal;
    v_TexCoord = a_TexCoord;
}


#type fragment
#version 440 core

layout(location = 0) out vec4 color;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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

    vec3 ambient = light.ambient * u_Material.ambient;
    vec3 diffuse = light.diffuse * diff * u_Material.diffuse;
    vec3 specular = light.specular * spec * u_Material.specular;

    return (ambient + diffuse + specular);
}