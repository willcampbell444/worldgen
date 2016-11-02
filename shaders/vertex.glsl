#version 150

in vec3 position;
in vec3 normal;

out vec3 Normal;
out vec3 FragPos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 flip;

void main() {
	Normal = normal*flip;
	FragPos = position*flip;
	gl_Position = projection * view * vec4(position*flip, 1.0);
}