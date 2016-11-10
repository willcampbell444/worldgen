#version 150

in vec3 position;

out vec3 pos;
out float height;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
	height = position.y;
	pos = (view * model * vec4(position, 1)).xyz;
	gl_Position = projection * view * model * vec4(position, 1.0);
}