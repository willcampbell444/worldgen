#ifndef VISUALIZE_H
#define VISUALIZE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shaders.h>
#include <groundGen.h>

class Visualize {
public:
	Visualize();
	void draw();
	void update();
	GLFWwindow* getWindow();
private:
	glm::mat4 _proj, _view;
    glm::vec3 _cameraPos;
    float _viewAngle = 0;
    GLFWwindow* _window;
    Shaders* _shader;

    GroundGen _ground;
    GLuint _vertexArrayObject;
};

#endif