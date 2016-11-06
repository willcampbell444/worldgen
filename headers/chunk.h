#ifndef CHUNK_H
#define CHUNK_H

#include <groundGen.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const int CHUNK_WIDTH = 16;

class Chunk {
public:
	Chunk(int x, int y, float*** adjacent, GroundGen generator);
	~Chunk();
	void draw();
	float* getHeightMap();
private:
	float** _heightMap;

	GLuint _VAO;
};

#endif