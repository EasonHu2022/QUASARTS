#version 330

layout(location = 0) in vec2 position;

out vec2 textureCoords;

uniform mat4 projection;
uniform mat4 view;

void main(void){

	textureCoords = position + vec2(0.5, 0.5);
	textureCoords.y = 1.0 - textureCoords.y;
	
	gl_Position = projection * view * vec4(position, 0.0, 1.0);

}