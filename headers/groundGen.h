#ifndef GROUND_GEN_H
#define GROUND_GEN_H

#include <vector>
#include <iostream>
#include <random>
#include <ctime>

class GroundGen{
public:
	// w must be a power of two plus 1
	GroundGen();
	void generate(int w);
	int getWidth();
	float getHeight(int x, int z);
private:
	void diamond(int centerX, int centerZ, int size);
	void square(int centerX, int centerZ, int size);
	int loopCoord(int c);
	float randomDisplacement(int size);

	int _width;
	std::vector<std::vector<float>> _heightMap;

	std::default_random_engine numGenerator;
	std::uniform_real_distribution<float> randFloats;
};

#endif