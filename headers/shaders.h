#ifndef SHADERS_H
#define SHADERS_H

#include <vector>
#include <GL/glew.h>
#include <fstream>
#include <string>

class Shaders {
public:
    ~Shaders();
    bool loadShader(GLenum shaderType, const char* filename);
    bool createProgram();
    void use();
    GLuint getAttributeLocation(const char* name);
    GLuint getUniformLocation(const char* name);
private:
    std::vector<GLuint> _shaders;
    GLuint _program;
};

#endif