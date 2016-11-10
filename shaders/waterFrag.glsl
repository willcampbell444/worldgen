#version 150

out vec4 outColor;

in vec3 pos;
in float height;

void main() {
	float l = length(pos);
	l = l-180;
	float fog = 1-clamp(exp(-l*0.2), 0.0, 1.0);

	// float trans = (-height+2)/20;
	float trans = 0.1;

	outColor = vec4(mix(vec3(0, 0, 1), vec3(16.0f/256.0f, 136.0f/256.0f, 136.0f/256.0f), 1), trans);
	// outColor = vec4(0, 0, 1, 0.4);
}