#version 150

out vec4 outColor;

in vec2 TexCoord;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gColor;
uniform sampler2D ssao;

uniform vec3 lightDirection;

uniform vec3 color;

void main() {
	vec3 color = texture(gColor, TexCoord).rgb;
	vec3 pos = texture(gPosition, TexCoord).rgb;
	vec3 normal = texture(gNormal, TexCoord).rgb;
	float specularStrength = texture(gColor, TexCoord).a;
	float ssaoVal = texture(ssao, TexCoord).r;

	if (normal == vec3(0, 0, 0)) {
		discard;
	}

	vec3 ambient = 0.3 * ssaoVal * vec3(1);

	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightDirection);
	vec3 diffuse = 0.45*max(dot(norm, -lightDirection), 0.0) * vec3(1);

	vec3 reflectDir = reflect(lightDirection, norm);
	vec3 viewDir = normalize(-pos);
	vec3 specular = specularStrength*pow(max(dot(normalize(-pos), reflectDir), 0), 16)*vec3(1);

	float l = length(pos);
	l = l-180;
	float fog = 1-clamp(exp(-l*0.2), 0.0, 1.0);

	outColor = vec4(mix((ambient + diffuse + specular)*color, vec3(16.0f/256.0f, 136.0f/256.0f, 136.0f/256.0f), fog), 1);
	// outColor = vec4(mix(norm, vec3(16.0f/256.0f, 136.0f/256.0f, 136.0f/256.0f), fog), 1);
}