#include <visualize.h>

const int width = 129;

Visualize::Visualize() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    _window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);

    glfwMakeContextCurrent(_window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    _proj = glm::perspective(glm::radians(50.0f), 800.0f / 600.0f, 1.0f, 100.0f);
    _cameraPos = glm::vec3(-10, 15, 0);
    _view = glm::lookAt(
        _cameraPos,
        glm::vec3(_cameraPos.x+10*cos(glm::radians(_viewAngle)), _cameraPos.y-5, _cameraPos.z+10*sin(glm::radians(_viewAngle))),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    glClearColor(16.0f/256.0f, 136.0f/256.0f, 136.0f/256.0f, 1.0f);

    _shader = new Shaders();

    if (!_shader->loadShader(GL_VERTEX_SHADER, "shaders/vertex.glsl")) {
        std::cout << "Vertex Shader Failed To Compile" << std::endl;
    }
    if (!_shader->loadShader(GL_FRAGMENT_SHADER, "shaders/fragment.glsl")) {
        std::cout << "Fragment Shader Failed To Compile" << std::endl;
    }
    _shader->createProgram();

    _ground.generate(width);

    glm::vec3 verticies[2*3*2*(width-1)*(width-1)];
    int count = 0;

    glm::vec3 v1, v2, v3, normal;

    glm::vec3 **normals;
    normals = new glm::vec3*[width];
    for (int x = 0; x < width; x++) {
        normals[x] = new glm::vec3[width];
        for (int z = 0; z < width; z++) {
            normals[x][z] = glm::vec3(0, 0, 0);
        }
    }

    for (int x = 0; x < width-1; x++) {
        for (int z = 0; z < width-1; z++) {
            // triangle 1
            v1 = glm::vec3(x, _ground.getHeight(x, z), z);
            v2 = glm::vec3(x+1, _ground.getHeight(x+1, z), z);
            v3 = glm::vec3(x, _ground.getHeight(x, z+1), z+1);
            normal = glm::cross(v1, v2);
            normals[x][z] += normal;
            normals[x+1][z] += normal;
            normals[x][z+1] += normal;
            verticies[count++] = v1;
            count++;
            verticies[count++] = v2;
            count++;
            verticies[count++] = v3;
            count++;
            // triangle 2
            v1 = glm::vec3(x+1, _ground.getHeight(x+1, z), z);
            v2 = glm::vec3(x+1, _ground.getHeight(x+1, z+1), z+1);
            v3 = glm::vec3(x, _ground.getHeight(x, z+1), z+1);
            normal = glm::cross(v3, v1);
            normals[x+1][z] += normal;
            normals[x+1][z+1] += normal;
            normals[x][z+1] += normal;
            verticies[count++] = v1;
            count++;
            verticies[count++] = v2;
            count++;
            verticies[count++] = v3;
            count++;
        }
    }

    count = 0;

    for (int x = 0; x < width-1; x++) {
        for (int z = 0; z < width-1; z++) {
            count++;
            verticies[count++] = normals[x][z];
            count++;
            verticies[count++] = normals[x+1][z];
            count++;
            verticies[count++] = normals[x][z+1];

            count++;
            verticies[count++] = normals[x+1][z];
            count++;
            verticies[count++] = normals[x+1][z+1];
            count++;
            verticies[count++] = normals[x][z+1];
        }
    }

    glGenVertexArrays(1, &_vertexArrayObject);
    glBindVertexArray(_vertexArrayObject);

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*2*3*2*(width-1)*(width-1), verticies, GL_STATIC_DRAW);

    GLuint attrib = _shader->getAttributeLocation("position");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), 0);

    attrib = _shader->getAttributeLocation("normal");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
}

GLFWwindow* Visualize::getWindow() {
    return _window;
}

void Visualize::update() {
    glfwPollEvents();

    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
        _cameraPos.z += 0.1f*sin(glm::radians(_viewAngle-90));
        _cameraPos.x += 0.1f*cos(glm::radians(_viewAngle-90));
    }
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
        _cameraPos.z -= 0.1f*sin(glm::radians(_viewAngle-90));
        _cameraPos.x -= 0.1f*cos(glm::radians(_viewAngle-90));
    }
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
        _cameraPos.z += 0.1f*sin(glm::radians(_viewAngle));
        _cameraPos.x += 0.1f*cos(glm::radians(_viewAngle));
    }
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
        _cameraPos.z -= 0.1f*sin(glm::radians(_viewAngle));
        _cameraPos.x -= 0.1f*cos(glm::radians(_viewAngle));
    }
    if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS) {
        _cameraPos.y += 0.1f;
    }
    if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        _cameraPos.y -= 0.1f;
    }
    if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS) {
        _viewAngle += 2;
    }
    if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS) {
        _viewAngle -= 2;
    }

    _view = glm::lookAt(
        _cameraPos,
        glm::vec3(_cameraPos.x+10*cos(glm::radians(_viewAngle)), _cameraPos.y-5, _cameraPos.z+10*sin(glm::radians(_viewAngle))),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

void Visualize::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader->use();
    glUniformMatrix4fv(_shader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(_view));
    glUniformMatrix4fv(_shader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(_proj));
    glUniform3f(_shader->getUniformLocation("color"), 45.0f/256.0f, 136.0f/256.0f, 45.0f/256.0f);
    glUniform3f(_shader->getUniformLocation("cameraPosition"), _cameraPos.x, _cameraPos.y, _cameraPos.z);
    glUniform3f(_shader->getUniformLocation("lightDirection"), -2, -5, 2);

    glBindVertexArray(_vertexArrayObject);

    glm::vec3 flip = glm::vec3(1, 1, 1);

    glUniform3f(_shader->getUniformLocation("flip"), flip.x, flip.y, flip.z);
    glDrawArrays(GL_TRIANGLES, 0, 2*3*(width-1)*(width-1));

    // flip.x = -1;
    // glUniform3f(_shader->getUniformLocation("flip"), flip.x, flip.y, flip.z);
    // glDrawArrays(GL_TRIANGLES, 0, 2*3*(width-1)*(width-1));

    // flip.z = -1;
    // glUniform3f(_shader->getUniformLocation("flip"), flip.x, flip.y, flip.z);
    // glDrawArrays(GL_TRIANGLES, 0, 2*3*(width-1)*(width-1));

    // flip.x = 1;
    // glUniform3f(_shader->getUniformLocation("flip"), flip.x, flip.y, flip.z);
    // glDrawArrays(GL_TRIANGLES, 0, 2*3*(width-1)*(width-1));

    glfwSwapBuffers(_window);
}