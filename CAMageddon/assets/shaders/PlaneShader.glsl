//Plane Shader

#type vertex
#version 440 core

layout (location =0) in vec3 a_Position;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_ModelMatrix;

void main()
{
    gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(a_Position, 1.0f);
}

#type tessControl
#version 440 core
layout(vertices  = 4) out;

void main()
{
    gl_TessLevelInner[0] = 10;
    gl_TessLevelInner[1] = 10;

    gl_TessLevelOuter[0] = 10;
    gl_TessLevelOuter[1] = 10;
    gl_TessLevelOuter[2] = 10;
    gl_TessLevelOuter[3] = 10;

    gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}


#type tessEval
#version 440 core

layout(quads, equal_spacing, cw) in;

void main()
{
    vec4 bottomLeft = gl_in[0].gl_Position;
    vec4 bottomRight = gl_in[1].gl_Position;
    vec4 topRight = gl_in[2].gl_Position; 
    vec4 topLeft = gl_in[3].gl_Position;
    
    vec4 p1 = mix(bottomLeft,bottomRight,gl_TessCoord.x);
    vec4 p2 = mix(topLeft,topRight,gl_TessCoord.x);

    gl_Position = mix(p1,p2,gl_TessCoord.y);

}

#type fragment
#version 440 core

layout(location = 0) out vec4 color;

uniform vec4 u_Color;

void main()
{
    color = u_Color;
}
