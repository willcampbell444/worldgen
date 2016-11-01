#include <groundGen.h>

GroundGen::GroundGen(): randFloats(0, 1) {}

void GroundGen::generate(int w) {
	_width = w;
	_heightMap = std::vector<std::vector<float>>(_width, std::vector<float>(_width, 0));
	_heightMap[_width/2][_width/2] = 10;

	numGenerator.seed(time(NULL));
	int size = _width/2;
	while (size > 0) {
		for (int i = size; i < _width; i += size*2) {
			for (int j = size; j < _width; j += size*2) {
				diamond(i, j, size);
			}
		}
		for (int i = 0; i < _width; i += size) {
			for (int j = (i+size) % (size*2); j <= _width; j += size*2) {
				square(i, j, size);
			}
		}

		size = size/2;
	}
}

void GroundGen::diamond(int centerX, int centerZ, int size) {
	std::cout << "diamond: " << centerX << ", " << centerZ << ", " << size << std::endl;
	float averageHeight = 0;
	averageHeight += _heightMap[centerX-size][centerZ-size]/4.0f;
	averageHeight += _heightMap[centerX+size][centerZ-size]/4.0f;
	averageHeight += _heightMap[centerX+size][centerZ+size]/4.0f;
	averageHeight += _heightMap[centerX-size][centerZ+size]/4.0f;

	_heightMap[centerX][centerZ] += averageHeight + randomDisplacement(size);
}

void GroundGen::square(int centerX, int centerZ, int size) {
	float averageHeight = 0;
	averageHeight += _heightMap[loopCoord(centerX-size)][loopCoord(centerZ)]/4.0f;
	averageHeight += _heightMap[loopCoord(centerX+size)][loopCoord(centerZ)]/4.0f;
	averageHeight += _heightMap[loopCoord(centerX)][loopCoord(centerZ+size)]/4.0f;
	averageHeight += _heightMap[loopCoord(centerX)][loopCoord(centerZ-size)]/4.0f;
	std::cout << "square: " << centerX << ", " << centerZ << ", " << size << " = " << averageHeight << std::endl;

	_heightMap[centerX][centerZ] += averageHeight + randomDisplacement(size/1.5f);
}

int GroundGen::loopCoord(int x) {
	if (x < 0) {
		return _width-1+x;
	} else if (x >= _width) {
		return x - _width + 1;
	}
	return x;
}

float GroundGen::randomDisplacement(int size) {
	return randFloats(numGenerator)*size*5;
}

float GroundGen::getHeight(int x, int z) {
	return _heightMap[x][z];
}