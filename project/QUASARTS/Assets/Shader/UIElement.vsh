#version 330

layout(location = 0) in vec3 position;

out vec2 textureCoords;

void main(void){
	textureCoords = vec2(
		(gl_VertexID << 1) & 2,
		gl_VertexID & 2
	);

	gl_Position = vec4(
		textureCoords * 2.f - 1.f,
		0.f,
		1.f
	);

	gl_Position.y *= -1.f;
}
