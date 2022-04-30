#version 420 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace[5];
} fs_in;

uniform vec3 viewPos;

uniform sampler2DArray shadowMap;

//uniform sampler2D singleShadowMap;

struct LightData
{   
     vec4 ambient;
     vec4 diffuse;
     vec4 specular ;
     vec4 positon;
     mat4 lightSpaceMatrix;
     float type;
};

layout(std140, binding = 1) uniform Lightinfo
{    
    LightData lights[5];
    float countLight;
};

float ShadowCalculation(vec4 fragPosLightSpace,int layer,vec3 normal, vec3 lightdir)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + vec3(0.5f,0.5f,0.5f);
    float closestDepth = texture(shadowMap, vec3(projCoords.xy,layer)).r;
    float currentDepth = projCoords.z;
    //avoid acne
    float bias = max(0.05 * (1.0 - dot(normal, lightdir)), 0.005);
    float shadow = currentDepth - bias > closestDepth  ? 0.7 : 0.0;

    return shadow;
}


void main()
{  
    //now only support point light

    vec3 color = vec3(1.0f,1.0f,1.0f);//texture(floorTexture, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.1 * color;
    vec3 normal = normalize(fs_in.Normal);

    //per light attribution
    FragColor = vec4(0.0f,0.0f,0.0f,1.0f);
    for(int i = 0;i < countLight;i++)
    {
        vec3 lightPos = lights[i].positon.xyz;
        // diffuse
        vec3 lightDir = normalize(lightPos - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * color;
        // specular
        vec3 viewDir = normalize(viewPos - fs_in.FragPos);
        //vec3 reflectDir = reflect(-lightDir, normal);
        float spec = 0.0;
        
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

        vec3 specular = vec3(0.3) * spec; // assuming bright white light color

        //calculate shadow
        float shadow = ShadowCalculation(fs_in.FragPosLightSpace[i],i,fs_in.Normal,lightDir);    

        //FragColor += fs_in.FragPosLightSpace[i];
        FragColor += vec4(ambient + (1-shadow) * (diffuse + specular), 0.0);
        
    }

    FragColor.w = 1.0f;

} 


