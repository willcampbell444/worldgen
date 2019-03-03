#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <list>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shaders.h>
#include <groundGen.h>
#include <chunk.h>
#include <random>
#include <string>

const float VIEW_DISTANCE = 500.0f;
const int GRID_WIDTH = 13;

class Visualize {
public:
	Visualize();
	void draw();
	void update(std::list<std::unique_ptr<Task>> &newTasks);
	GLFWwindow* getWindow();
private:
	ChunkGrid _chunkGrid;
	glm::mat4 _proj, _view;
    glm::vec2 _cameraChunk;
    glm::vec3 _cameraPos;
    float _viewAngle = 0;
    float _targetY = -5;
    float _lastTime;
    GLFWwindow* _window;
    Shaders* _gShader;
    Shaders* _ssaoShader;
    Shaders* _blurShader;
    Shaders* _lightShader;
    Shaders* _waterShader;
    Shaders* _vignetteShader;

    GroundGen _ground;
    GLuint _gBuffer, _ssaoBuffer, _blurBuffer, _screenBuffer;
    GLuint _gPosition, _gNormal, _gColorSpec, _noiseTexture, _ssaoColorBuffer, _blurColorBuffer, _screenColorBuffer;
    GLuint _waterVAO, _screenVAO;
};

#endif
