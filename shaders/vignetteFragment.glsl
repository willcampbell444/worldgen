#version 150

in vec2 TexCoords;

out vec4 outColor;

uniform sampler2D screen;

const int WINDOW_WIDTH = 2000;
const int WINDOW_HEIGHT = 1600;
const float WINDOW_CROSS = 2561.0;

void main() {
	float strength = 0;

	// window corners
	for (int x = 0; x <= WINDOW_WIDTH; x += WINDOW_WIDTH) {
		for (int y = 0; y <= WINDOW_HEIGHT; y += WINDOW_HEIGHT) {
			float dist = length(gl_FragCoord.xy - vec2(x, y));
			strength += exp(dist/(WINDOW_CROSS/2));
		}
	}
	strength = 8/strength;

	outColor = vec4(mix(texture(screen, TexCoords).rgb, vec3(0, 0, 0), 1-strength), 1);
	// outColor = vec4(texture(screen, TexCoords).rgb, 1);
	// outColor = vec4(strength, strength, strength, 1);
}