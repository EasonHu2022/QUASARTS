#version 330

in vec2 textureCoords;

uniform sampler2D uiElementTexture;

out vec4 out_colour;

void main(void){
	vec4 fragColour = texture(uiElementTexture, textureCoords);
	if (fragColour.a < 0.001)
	{
		discard;
	}

	out_colour = fragColour;
}
