#version 150

in vec3 position;
in vec2 texPos;

out vec2 TexCoords;

void main() {
	TexCoords = texPos;
	gl_Position = vec4(position, 1);
}