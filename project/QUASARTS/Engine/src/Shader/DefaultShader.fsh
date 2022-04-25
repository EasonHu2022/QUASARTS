#version 420 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

//uniform sampler2D floorTexture;
//uniform vec3 lightPos;
uniform vec3 viewPos;
//uniform bool blinn;

struct LightData
{   
     vec3 ambient;
     vec3 diffuse;
     vec3 specular ;
     vec3 positon;
     float type;
};

layout(std140, binding = 1) uniform Lightinfo
{    
    LightData lights[10];
    float countLight;
};

void main()
{  
    //now only support point light

    vec3 color = vec3(1.0f,1.0f,1.0f);//texture(floorTexture, fs_in.TexCoords).rgb;
    // ambient
    vec3 ambient = 0.05 * color;
    vec3 normal = normalize(fs_in.Normal);

    //per light attribution
    vec4 colorPerLight;
    FragColor = vec4(0.0f,0.0f,0.0f,1.0f);
    for(int i = 0;i < countLight;i++)
    {
        vec3 lightPos = lights[0].positon;
        // diffuse
        vec3 lightDir = normalize(lightPos - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        vec3 diffuse = diff * color;
        // specular
        vec3 viewDir = normalize(viewPos - fs_in.FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = 0.0;
        
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

        vec3 specular = vec3(0.3) * spec; // assuming bright white light color
        FragColor += vec4(ambient + diffuse + specular, 0.0);
        //FragColor = vec4(0.2f,0.2f,0.2f,1.0f);
        //FragColor += vec4(normal, 1.0);
    }
    FragColor.w = 1.0f;
} 


/*
#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;


out vec4 FragColor;
void main()
{
	FragColor = vec4(0.2f,0.2f,0.2f,1.0f);
};*/