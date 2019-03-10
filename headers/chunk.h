#ifndef CHUNK_H
#define CHUNK_H

#include <deque>
#include <mutex>
#include <list>
#include <groundGen.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <shaders.h>
#include "taskQueue.h"

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

class ShiftTask;

class ChunkGrid {
	friend class ShiftTask;

	int _xWidth, _zWidth;

public:
	std::mutex lock;
	glm::vec2 _centerChunk;
	glm::vec2 _pendingCenter;
	std::deque<std::deque<std::unique_ptr<Chunk>>> grid;

	void fill(int xWidth, int zWidth, GroundGen *groundGen) {
		_centerChunk = glm::vec2(0.f, 0.f);
		_pendingCenter = _centerChunk;
		std::lock_guard<std::mutex> l(lock);
		_xWidth = xWidth;
		_zWidth = zWidth;

		grid.clear();
		for (int x = -xWidth/2; x <= xWidth/2; x++) {
			grid.emplace_back();
			for (int z = -zWidth/2; z <= zWidth/2; z++) {
				grid.back().push_back(std::make_unique<Chunk>(
					_centerChunk.x + x,
					_centerChunk.y + z,
					groundGen));
			}
		}
	}

	void shift(glm::vec2 newCenterChunk, std::list<std::unique_ptr<Task>> &newTasks, GroundGen *groundGen);

	void _shift(glm::vec2 newCenterChunk, GroundGen *groundGen) {
		if (newCenterChunk != _centerChunk) {
			const auto diff = newCenterChunk - _centerChunk;
			_centerChunk = newCenterChunk;

			if (diff.x < 0) {
				for (int i = 0; i < -diff.x; i++) {
					grid.pop_back();
					grid.emplace_front();

					for (int z = -_zWidth/2; z <= _zWidth/2; z++) {
						grid.front().push_back(std::make_unique<Chunk>(
							_centerChunk.x - diff.x - i - 1 - _xWidth/2,
							_centerChunk.y + z,
							groundGen));
					}
				}
			} else if (diff.x > 0) {
				for (int i = 0; i < diff.x; i++) {
					grid.pop_front();
					grid.emplace_back();

					for (int z = -_zWidth/2; z <= _zWidth/2; z++) {
						grid.back().push_back(std::make_unique<Chunk>(
							_centerChunk.x - diff.x + i + 1 + _xWidth/2,
							_centerChunk.y + z,
							groundGen));
					}
				}
			}

			if (diff.y < 0) {
				int x = _centerChunk.x - diff.x - _xWidth/2;
				for (auto& chunkList: grid) {
					for (int i = 0; i < -diff.y; i++) {
						chunkList.pop_back();
						chunkList.push_front(std::make_unique<Chunk>(
							x,
							_centerChunk.y - diff.y - i - 1 - _zWidth/2,
							groundGen));
					}
					x++;
				}
			} else if (diff.y > 0) {
				int x = _centerChunk.x - diff.x - _xWidth/2;
				for (auto& chunkList: grid) {
					for (int i = 0; i < diff.y; i++) {
						chunkList.pop_front();
						chunkList.push_back(std::make_unique<Chunk>(
							x,
							_centerChunk.y - diff.y + i + 1 + _zWidth/2,
							groundGen));
					}
					x++;
				}
			}
		}
	}
};

class ShiftTask: public Task {
	ChunkGrid& _chunkGrid;
	glm::vec2 _destination;
	GroundGen* _groundGen;
	
public:
	ShiftTask(ChunkGrid& g, glm::vec2 dest, GroundGen *gGen):
		_chunkGrid(g), _destination(dest), _groundGen(gGen) { }
	void execute() override {
		std::unique_lock<std::mutex> l(_chunkGrid.lock, std::try_to_lock);
		if (l.owns_lock()) {
			_chunkGrid._shift(_destination, _groundGen);
		}
	}
};

#endif
