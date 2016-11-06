#version 150

in vec3 position;
in vec3 normal;
in vec3 color;

out vec3 Normal;
out vec3 FragPos;
out vec3 Color;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main() {
	mat3 normalMatrix = transpose(inverse(mat3(view * model)));
    Normal = normalMatrix * normal;
    Color = color;
	FragPos = (view * model * vec4(position, 1.0)).xyz;
	gl_Position = projection * view * model * vec4(position, 1.0);
}