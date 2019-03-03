#version 330

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 Color;

const float NEAR = 1;
const float FAR = 200;

void main() {
	gPosition.rgb = FragPos;
	float z = gl_FragCoord.z*2 - 1;
	gPosition.a = (2*NEAR*FAR) / (FAR + NEAR - z*(FAR-NEAR));
	gNormal = Normal;
	gColor = vec4(Color, 0.5);
}
