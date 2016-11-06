#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shaders.h>
#include <groundGen.h>
#include <random>
#include <string>

class Visualize {
public:
	Visualize();
	void draw();
	void update();
	GLFWwindow* getWindow();
private:
	glm::vec3 getColor(float height, float normalY);
	glm::mat4 _proj, _view;
    glm::vec3 _cameraPos;
    float _viewAngle = 0;
    float _targetY = -5;
    int width = 257;
    GLFWwindow* _window;
    Shaders* _gShader;
    Shaders* _ssaoShader;
    Shaders* _blurShader;
    Shaders* _lightShader;
    Shaders* _waterShader;

    GroundGen _ground;
    GLuint _vertexArrayObject;
    GLuint _gBuffer, _ssaoBuffer;
    GLuint _gPosition, _gNormal, _gColorSpec, _noiseTexture, _ssaoColorBuffer;
    GLuint _waterVAO, _screenVAO;
};

#endif