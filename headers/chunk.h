#ifndef CHUNK_H
#define CHUNK_H

#include <groundGen.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <shaders.h>

const int CHUNK_WIDTH = 32;

class Chunk {
public:
	Chunk(int x, int y, GroundGen* generator);
	~Chunk();
	void draw();
private:
	glm::vec3 getColor(float height, float normalY);
	GLuint _VAO, _VBO;
};

#endif