#version 150

in vec3 Normal;
in vec3 FragPos;

out vec4 outColor;

float ambientStrength = 0.5;

vec3 lightDirection;

uniform vec3 color;
uniform vec3 cameraPosition;

void main() {
	vec3 ambient = ambientStrength * vec3(1);

	vec3 norm = normalize(Normal);
	lightDirection = normalize(vec3(1, -3, 1));
	vec3 diffuse = 0.45*max(dot(norm, -lightDirection), 0.0) * vec3(1);

	vec3 reflectDir = reflect(lightDirection, norm);
	vec3 viewDir = normalize(cameraPosition);
	vec3 specular = 0.1*pow(max(dot(viewDir, reflectDir), 0), 256)*vec3(1);

	float l = length(cameraPosition-FragPos);
	float fog = 0;
	if (l > 95) {
		l = l - 95;
		fog = l/5;
	}

	outColor = vec4(mix((ambient + diffuse + specular)*color, vec3(16.0f/256.0f, 136.0f/256.0f, 136.0f/256.0f), fog), 1);
}