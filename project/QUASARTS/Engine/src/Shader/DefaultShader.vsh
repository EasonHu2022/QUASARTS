#version 420 core

//data from application
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aNormal;
layout(location = 4) in vec3 aTangent;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;


out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace[5] ;
} vs_out;

//data from UBO
layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

struct LightData
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 positon;
    mat4 lightSpaceMatrix;
    float type;
};

layout(std140, binding = 1) uniform Lightinfo
{
    LightData lights[5];
    float countLight;
};


void main()
{
    vs_out.FragPos = aPos;
    vs_out.Normal = aNormal;
    vs_out.TexCoords = aTexCoords;
    for (int i = 0; i < countLight; i++)
    {
        vs_out.FragPosLightSpace[i] = lights[i].lightSpaceMatrix * vec4(aPos, 1.0);
    }  
   // vs_out.FragPosLightSpace = lights[0].lightSpaceMatrix * vec4(aPos, 1.0);
    //gl_Position = projection ;
     gl_Position = projection * view * model * vec4(aPos, 1.0);
    //gl_Position = view * model * vec4(aPos, 1.0);
    //gl_Position =  vec4(aPos, 1.0);
}

//#version 420 core
//layout (location = 0) in vec3 aPos;
//void main()
//{
//	gl_Position = vec4(aPos,1.0);
//};