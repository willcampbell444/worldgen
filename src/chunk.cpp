#include <chunk.h>

Chunk::Chunk(int x, int y, float*** adjacent, GroundGen generator) {
	generator.generate(CHUNK_WIDTH+1, float*** adjacent, int x, int y);
	_heightMap = generator.getHeightMap;
	
}

Chunk::~Chunk() {
	delete[] _heightMap;
}

void Chunk::draw() {

}

float* Chunk::getHeightMap() {

}