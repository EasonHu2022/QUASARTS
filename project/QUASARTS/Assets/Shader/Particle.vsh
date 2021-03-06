#version 330

layout(location = 0) in vec3 position;

out vec2 textureCoords1;
out vec2 textureCoords2;
out float blend;

uniform mat4 projection;
uniform mat4 view;

uniform vec2 texOffset1;
uniform vec2 texOffset2;
uniform vec2 texCoordInfo;

void main(void){

	vec2 textureCoords = position.xy + vec2(0.5, 0.5);
	textureCoords.y = 1.0 - textureCoords.y;
	textureCoords /= texCoordInfo.x;
	textureCoords1 = textureCoords + texOffset1;
	textureCoords2 = textureCoords + texOffset2;
	blend = texCoordInfo.y;
	gl_Position = projection * view * vec4(position, 1.0);

}
