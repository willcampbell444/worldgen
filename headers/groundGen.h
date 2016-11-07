#ifndef GROUND_GEN_H
#define GROUND_GEN_H

#include <vector>
#include <iostream>
#include <random>
#include <ctime>

const float HEIGHT_MULT = 120.0f;
const float BASE_HEIGHT = 20.0f;
const float TILE_SIZE   = 512.0f;
const int   OCTAVES     = 16;
const float PERSISTANCE = 0.55f;

class GroundGen{
public:
	// w must be a power of two plus 1
	GroundGen();
	void generate(int w);
	int getWidth();
	float getHeight(int x, int z);
private:
	double lerp(double a, double b, double x);
	double fade(double t);
	double grad(int hash, double x, double y);
	double perlin(double x, double y);
	double octPerlin(double x, double y, int octaves, double persistance);

	int p[512];

	int _width;
	float** _heightMap;

	std::default_random_engine numGenerator;
};

#endif