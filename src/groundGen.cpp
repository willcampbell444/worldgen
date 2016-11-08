#include <groundGen.h>

GroundGen::GroundGen() {
	// random list of integers between 0 and 255 inclusive
	numGenerator.seed(time(NULL));

	for (int i = 0; i < 256; i++) {
		p[i] = i;
	}
	for (int i = 1; i < 255; i++) {
		std::uniform_int_distribution<int> randFloats(i, 255);
		int r = randFloats(numGenerator);
		int chose = p[r];
		p[r] = p[i];
		p[i] = chose;
	}
	for (int i = 0; i < 256; i++) {
		p[i + 256] = p[i];
	}

}

void GroundGen::generate(int w) {
	// _width = w;
	// _heightMap = new float*[_width];
	// for (int i = 0; i < _width; i++) {
	// 	_heightMap[i] = new float[_width];
	// 	for (int j = 0; j < _width; j++) {
	// 		_heightMap[i][j] = HEIGHT_MULT*octPerlin((float)i/TILE_SIZE, (float)j/TILE_SIZE, OCTAVES, PERSISTANCE) + BASE_HEIGHT;
	// 	}
	// }
}

double GroundGen::octPerlin(double x, double y, int octaves, double persistance) {
	double total = 0;
	double frequency = 1;
	double amplitude = 1;
	double maxVal = 0;

	for (int i = 0; i < octaves; i++) {
		total += perlin(x*frequency, y*frequency)*amplitude;
		amplitude = amplitude*persistance;
		frequency = frequency*2;
		maxVal += amplitude;
	}

	return total/maxVal;
}

double GroundGen::perlin(double x, double y) {
	int xi = (int)x & 255;
	int yi = (int)y & 255;
	double xf = x-(int)x;
	double yf = y-(int)y;

	double u = fade(xf);
	double v = fade(yf);

	int aa, ab, ba, bb;
	aa = p[p[xi] + yi];
	ab = p[p[xi] + yi + 1];
	ba = p[p[xi + 1] + yi];
	bb = p[p[xi + 1] + yi + 1];

	double x1, x2;
	x1 = lerp(
		grad(aa, xf, yf),
		grad(ba, xf - 1, yf),
		u
	);
	x2 = lerp(
		grad(ab, xf, yf - 1),
		grad(bb, xf - 1, yf - 1),
		u
	);

	return lerp(x1, x2, v);
}

double GroundGen::grad(int hash, double x, double y) {
	return ((hash & 1) ? x : -x) + ((hash & 2) ? y : -y);
}

double GroundGen::fade(double t) {
	return t*t*t*(t*(t*6 - 15) + 10);
}

double GroundGen::lerp(double a, double b, double x) {
	return a + x*(b - a);
}

float GroundGen::getHeight(int x, int z) {
	return HEIGHT_MULT*octPerlin((float)x/TILE_SIZE, (float)z/TILE_SIZE, OCTAVES, PERSISTANCE) + BASE_HEIGHT;
}