#include <visualize.h>

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

Visualize::Visualize() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 8);

    _window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL", nullptr, nullptr);

    glfwMakeContextCurrent(_window);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_CULL_FACE);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    _proj = glm::perspective(glm::radians(50.0f), (float)SCR_WIDTH / SCR_HEIGHT, 1.0f, 150.0f);
    _cameraPos = glm::vec3(width*0.4, 15, width*0.4);
    _view = glm::lookAt(
        _cameraPos,
        glm::vec3(_cameraPos.x+10*cos(glm::radians(_viewAngle)), _cameraPos.y-5, _cameraPos.z+10*sin(glm::radians(_viewAngle))),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    _gShader = new Shaders();
    if (!_gShader->loadShader(GL_VERTEX_SHADER, "shaders/gVertex.glsl")) {
        std::cout << "G Vertex Shader Failed To Compile" << std::endl;
    }
    if (!_gShader->loadShader(GL_FRAGMENT_SHADER, "shaders/gFragment.glsl")) {
        std::cout << "G Fragment Shader Failed To Compile" << std::endl;
    }
    _gShader->createProgram();

    _lightShader = new Shaders();
    if (!_lightShader->loadShader(GL_VERTEX_SHADER, "shaders/lightVertex.glsl")) {
        std::cout << "Light Vertex Shader Failed To Compile" << std::endl;
    }
    if (!_lightShader->loadShader(GL_FRAGMENT_SHADER, "shaders/lightFragment.glsl")) {
        std::cout << "Light Fragment Shader Failed To Compile" << std::endl;
    }
    _lightShader->createProgram();
    _lightShader->use();
    glUniform1i(_lightShader->getUniformLocation("ssaoColor"), 0);
    glUniform1i(_lightShader->getUniformLocation("gPosition"), 1);
    glUniform1i(_lightShader->getUniformLocation("gNormal"), 2);
    glUniform1i(_lightShader->getUniformLocation("gColor"), 3);

    _waterShader = new Shaders();
    if (!_waterShader->loadShader(GL_VERTEX_SHADER, "shaders/waterVertex.glsl")) {
        std::cout << "Water Vertex Shader Failed To Compile" << std::endl;
    }
    if (!_waterShader->loadShader(GL_FRAGMENT_SHADER, "shaders/waterFrag.glsl")) {
        std::cout << "Water Fragment Shader Failed To Compile" << std::endl;
    }
    _waterShader->createProgram();

    _ssaoShader = new Shaders();
    if (!_ssaoShader->loadShader(GL_VERTEX_SHADER, "shaders/ssaoVertex.glsl")) {
        std::cout << "SSAO Vertex Shader Failed To Compile" << std::endl;
    }
    if (!_ssaoShader->loadShader(GL_FRAGMENT_SHADER, "shaders/ssaoFragment.glsl")) {
        std::cout << "SSAO Fragment Shader Failed To Compile" << std::endl;
    }
    if (!_ssaoShader->createProgram()) {
        std::cout << "SSAO Shader Failed To Link" << std::endl;
    }
    _ssaoShader->use();
    glUniform1i(_lightShader->getUniformLocation("gPosition"), 0);
    glUniform1i(_lightShader->getUniformLocation("gNormal"), 1);
    glUniform1i(_lightShader->getUniformLocation("texNoise"), 2);

    std::uniform_real_distribution<GLfloat> randomFloats(-1, 1);
    std::default_random_engine generator;
    std::vector<glm::vec3> kernel;

    for (int i = 0; i < 32; i++) {
        glm::vec3 peice = glm::vec3(
            randomFloats(generator),
            randomFloats(generator),
            randomFloats(generator)/2 + 1
        );
        peice = glm::normalize(peice);
        peice = peice * (randomFloats(generator)/2 + 1);
        float scale = (float)i/32;
        scale = 0.1f + (scale*scale)*(1.0f-0.1f);
        kernel.push_back(peice*scale);
    }

    for (int i = 0; i < 32; i++) {
        glUniform3fv(_ssaoShader->getUniformLocation(("samples[" + std::to_string(i) + "]").c_str()), 1, &kernel[i][0]);
    }

    _ground.generate(width);

    glm::vec3 *verticies = new glm::vec3[2*3*3*2*(width-1)*(width-1)];
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
            v2 = glm::vec3(x, _ground.getHeight(x, z+1), z+1);
            v3 = glm::vec3(x+1, _ground.getHeight(x+1, z), z);
            normal = glm::cross(v1, v3);
            normals[x][z] += normal;
            normals[x][z+1] += normal;
            normals[x+1][z] += normal;
            verticies[count++] = v1;
            count++;
            count++;
            verticies[count++] = v2;
            count++;
            count++;
            verticies[count++] = v3;
            count++;
            count++;
            // triangle 2
            v1 = glm::vec3(x+1, _ground.getHeight(x+1, z), z);
            v2 = glm::vec3(x, _ground.getHeight(x, z+1), z+1);
            v3 = glm::vec3(x+1, _ground.getHeight(x+1, z+1), z+1);
            normal = glm::cross(v2, v1);
            normals[x+1][z] += normal;
            normals[x][z+1] += normal;
            normals[x+1][z+1] += normal;
            verticies[count++] = v1;
            count++;
            count++;
            verticies[count++] = v2;
            count++;
            count++;
            verticies[count++] = v3;
            count++;
            count++;
        }
    }

    count = 0;

    for (int x = 0; x < width-1; x++) {
        for (int z = 0; z < width-1; z++) {
            count++;
            verticies[count++] = glm::normalize(normals[x][z]);
            verticies[count++] = getColor(verticies[count-3].y, verticies[count-2].y);
            count++;
            verticies[count++] = glm::normalize(normals[x][z+1]);
            verticies[count++] = getColor(verticies[count-3].y, verticies[count-2].y);
            count++;
            verticies[count++] = glm::normalize(normals[x+1][z]);
            verticies[count++] = getColor(verticies[count-3].y, verticies[count-2].y);

            count++;
            verticies[count++] = glm::normalize(normals[x+1][z]);
            verticies[count++] = getColor(verticies[count-3].y, verticies[count-2].y);
            count++;
            verticies[count++] = glm::normalize(normals[x][z+1]);
            verticies[count++] = getColor(verticies[count-3].y, verticies[count-2].y);
            count++;
            verticies[count++] = glm::normalize(normals[x+1][z+1]);
            verticies[count++] = getColor(verticies[count-3].y, verticies[count-2].y);
        }
    }

    glGenVertexArrays(1, &_vertexArrayObject);
    glBindVertexArray(_vertexArrayObject);

    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*2*3*3*2*(width-1)*(width-1), verticies, GL_STATIC_DRAW);

    GLuint attrib = _gShader->getAttributeLocation("position");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), 0);

    attrib = _gShader->getAttributeLocation("normal");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

    attrib = _gShader->getAttributeLocation("color");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));

    float screenVerticies[30] = {
         1,  1, 0, 1, 1,
        -1, -1, 0, 0, 0,
         1, -1, 0, 1, 0,

         1,  1, 0, 1, 1,
        -1,  1, 0, 0, 1,
        -1, -1, 0, 0, 0
    };

    glGenVertexArrays(1, &_screenVAO);
    glBindVertexArray(_screenVAO);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*30, screenVerticies, GL_STATIC_DRAW);

    attrib = _lightShader->getAttributeLocation("position");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), 0);

    attrib = _lightShader->getAttributeLocation("texPos");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

    float waterVert[108] = {
         (float)width,  -12,  (float)width,
         (float)width,  -12, 0,
        0,  -12, 0,
         (float)width,  -12,  (float)width,
        0,  -12, 0,
        0,  -12,  (float)width,

         (float)width, -333,  (float)width,
         (float)width, -333, 0,
        0, -333, 0,
         (float)width, -333,  (float)width,
        0, -333, 0,
        0, -333,  (float)width,

         (float)width,  -12,  (float)width,
         (float)width, -333,  (float)width,
        0, -333,  (float)width,
         (float)width,  -12,  (float)width,
        0, -333,  (float)width,
        0,  -12,  (float)width,

         (float)width,  -12, 0,
         (float)width, -333, 0,
        0, -333, 0,
         (float)width,  -12, 0,
        0, -333, 0,
        0,  -12, 0,

         (float)width,  -12,  (float)width,
         (float)width, -333,  (float)width,
         (float)width, -333, 0,
         (float)width,  -12,  (float)width,
         (float)width, -333, 0,
         (float)width,  -12, 0,

        0,  -12,  (float)width,
        0, -333,  (float)width,
        0, -333, 0,
        0,  -12,  (float)width,
        0, -333, 0,
        0,  -12, 0,
    };

    glGenVertexArrays(1, &_waterVAO);
    glBindVertexArray(_waterVAO);

    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*108, waterVert, GL_STATIC_DRAW);

    attrib = _waterShader->getAttributeLocation("position");
    glEnableVertexAttribArray(attrib);
    glVertexAttribPointer(attrib, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);

    // Create G-buffer
    glGenFramebuffers(1, &_gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);

    glGenTextures(1, &_gPosition);
    glBindTexture(GL_TEXTURE_2D, _gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _gPosition, 0);

    glGenTextures(1, &_gNormal);
    glBindTexture(GL_TEXTURE_2D, _gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _gNormal, 0);

    glGenTextures(1, &_gColorSpec);
    glBindTexture(GL_TEXTURE_2D, _gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, _gColorSpec, 0);

    GLuint attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    GLuint rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "G-buffer is not complete" << std::endl;
    }

    std::vector<glm::vec3> ssaoNoise;
    for (GLuint i = 0; i < 16; i++)
    {
        glm::vec3 noise(
            randomFloats(generator) * 2.0 - 1.0, 
            randomFloats(generator) * 2.0 - 1.0, 
            0.0f); 
        ssaoNoise.push_back(noise);
    }

    glGenTextures(1, &_noiseTexture);
    glBindTexture(GL_TEXTURE_2D, _noiseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenFramebuffers(1, &_ssaoBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, _ssaoBuffer);

    glGenTextures(1, &_ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, _ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _ssaoColorBuffer, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "SSAO buffer is not complete" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    delete[] verticies;
}

glm::vec3 Visualize::getColor(float height, float normalY) {
    if (height < -10) {
        return glm::vec3(113.0f/256.0f, 110.0f/256.0f, 62.0f/256.0f);
    } else if (height < -8) {
        return glm::vec3(170.0f/256.0f, 163.0f/256.0f, 57.0f/256.0f);
    } else if (height < -5) {
        return glm::vec3(227.0f/256.0f, 215.0f/256.0f, 26.0f/256.0f);
    } else {
        return glm::vec3(45.0f/256.0f, 136.0f/256.0f, 45.0f/256.0f);
    }
}

GLFWwindow* Visualize::getWindow() {
    return _window;
}

void Visualize::update() {
    glfwPollEvents();

    if (glfwGetKey(_window, GLFW_KEY_A) == GLFW_PRESS) {
        _cameraPos.z += 1*sin(glm::radians(_viewAngle-90));
        _cameraPos.x += 1*cos(glm::radians(_viewAngle-90));
    }
    if (glfwGetKey(_window, GLFW_KEY_D) == GLFW_PRESS) {
        _cameraPos.z -= 1*sin(glm::radians(_viewAngle-90));
        _cameraPos.x -= 1*cos(glm::radians(_viewAngle-90));
    }
    if (glfwGetKey(_window, GLFW_KEY_W) == GLFW_PRESS) {
        _cameraPos.z += 1*sin(glm::radians(_viewAngle));
        _cameraPos.x += 1*cos(glm::radians(_viewAngle));
    }
    if (glfwGetKey(_window, GLFW_KEY_S) == GLFW_PRESS) {
        _cameraPos.z -= 1*sin(glm::radians(_viewAngle));
        _cameraPos.x -= 1*cos(glm::radians(_viewAngle));
    }
    if (glfwGetKey(_window, GLFW_KEY_UP) == GLFW_PRESS) {
        _cameraPos.y += 1;
    }
    if (glfwGetKey(_window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        _cameraPos.y -= 1;
    }
    if (glfwGetKey(_window, GLFW_KEY_R) == GLFW_PRESS) {
        _targetY += 0.1;
    }
    if (glfwGetKey(_window, GLFW_KEY_F) == GLFW_PRESS) {
        _targetY -= 0.1;
    }
    if (glfwGetKey(_window, GLFW_KEY_E) == GLFW_PRESS) {
        _viewAngle += 2;
    }
    if (glfwGetKey(_window, GLFW_KEY_Q) == GLFW_PRESS) {
        _viewAngle -= 2;
    }

    _view = glm::lookAt(
        _cameraPos,
        glm::vec3(_cameraPos.x+10*cos(glm::radians(_viewAngle)), _cameraPos.y+_targetY, _cameraPos.z+10*sin(glm::radians(_viewAngle))),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
}

void Visualize::draw() {
    // Render geometry
    glBindFramebuffer(GL_FRAMEBUFFER, _gBuffer);
    glClearColor(0, 0, 0, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _gShader->use();
    glUniformMatrix4fv(_gShader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(_view));
    glUniformMatrix4fv(_gShader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(_proj));

    glm::mat4 model = glm::scale(glm::mat4(1), glm::vec3(0.6, 0.6, 0.6));
    glUniformMatrix4fv(_gShader->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(_vertexArrayObject);

    glDrawArrays(GL_TRIANGLES, 0, 2*3*(width-1)*(width-1));
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ssao
    glBindFramebuffer(GL_FRAMEBUFFER, _ssaoBuffer);
    glClear(GL_COLOR_BUFFER_BIT);
    _ssaoShader->use();
    glUniformMatrix4fv(_ssaoShader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(_proj));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _noiseTexture);

    glBindVertexArray(_screenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // apply lighting
    glClearColor(16.0f/256.0f, 136.0f/256.0f, 136.0f/256.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    _lightShader->use();
    glUniformMatrix4fv(_lightShader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(_view));
    glUniformMatrix4fv(_lightShader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(_proj));
    glUniform3f(_lightShader->getUniformLocation("lightDirection"), -2, -5, 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _ssaoColorBuffer);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _gPosition);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, _gNormal);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, _gColorSpec);

    glBindVertexArray(_screenVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // draw the water
    // _waterShader->use();
    // glUniformMatrix4fv(_waterShader->getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    // glUniformMatrix4fv(_waterShader->getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(_view));
    // glUniformMatrix4fv(_waterShader->getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(_proj));
    // glBindVertexArray(_waterVAO);
    // glDrawArrays(GL_TRIANGLES, 0, 108);

    glfwSwapBuffers(_window);
}