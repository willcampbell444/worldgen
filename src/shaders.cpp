#include <shaders.h>

Shaders::~Shaders() {
    glDeleteProgram(_program);
    for (GLuint shader: _shaders) {
        glDeleteShader(shader);
    }
}

bool Shaders::loadShader(GLenum shaderType, const char* filename) {
    std::ifstream inVertex(filename);
    std::string shaderContents((std::istreambuf_iterator<char>(inVertex)), std::istreambuf_iterator<char>());
    const char* shaderSource = shaderContents.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);
    _shaders.push_back(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    return status == GL_TRUE;
}

void Shaders::createProgram() {
    _program = glCreateProgram();

    for (GLuint shader: _shaders) {
        glAttachShader(_program, shader);
    }

    glBindFragDataLocation(_program, 0, "outColor");
    glLinkProgram(_program);
}

void Shaders::use() {
    glUseProgram(_program);
}

GLuint Shaders::getAttributeLocation(const char* name) {
    return glGetAttribLocation(_program, name);
}

GLuint Shaders::getUniformLocation(const char* name) {
    return glGetUniformLocation(_program, name);
}