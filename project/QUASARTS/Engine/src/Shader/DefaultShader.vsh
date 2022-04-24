#version 420 core

//data from application
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
uniform mat4 model;

out VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

//data from UBO
layout(std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};


void main()
{
    vs_out.FragPos = aPos;
    vs_out.Normal = aNormal;
    vs_out.TexCoords = aTexCoords;

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