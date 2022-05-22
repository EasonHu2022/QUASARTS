#version 330 core
out vec4 FragColor;


in vec2 TexCoords;

uniform sampler2D _colorTexture;
uniform sampler2D postTexture;

uniform float exposure;

void main()
{   
    const float gamma = 2.2;
    vec3 bloomColor =  texture(postTexture, TexCoords).rgb;   
    vec3 col = texture(_colorTexture, TexCoords).rgb;
    col += bloomColor;
     // tone mapping
    vec3 result = vec3(1.0) - exp(-col * exposure);
    // also gamma correct while we're at it       
    //result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
   
}