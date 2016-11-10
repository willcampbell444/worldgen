#include <chunk.h>

Chunk::Chunk(int chunkX, int chunkY, GroundGen *generator) {
    glm::vec3 *verticies = new glm::vec3[2*3*3*2*(CHUNK_WIDTH-1)*(CHUNK_WIDTH-1)];
    int count = 0;

    glm::vec3 v1, v2, v3, normal;

    glm::vec3 **normals;
    normals = new glm::vec3*[CHUNK_WIDTH];
    for (int x = 0; x < CHUNK_WIDTH; x++) {
        normals[x] = new glm::vec3[CHUNK_WIDTH];
        for (int z = 0; z < CHUNK_WIDTH; z++) {
            normals[x][z] = glm::vec3(0, 0, 0);
        }
    }

    for (int x = 0; x < CHUNK_WIDTH-1; x++) {
        for (int z = 0; z < CHUNK_WIDTH-1; z++) {
            // triangle 1
            v1 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z), z);
            v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z+1);
            v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
            normal = glm::cross(v1 - v2, v1 - v3);
            normals[x][z] += normal;
            normals[x][z+1] += normal;
            normals[x+1][z] += normal;
            verticies[count++] = v1;
            verticies[count++] = normal;
            count++;
            verticies[count++] = v2;
            verticies[count++] = normal;
            count++;
            verticies[count++] = v3;
            verticies[count++] = normal;
            count++;
            // triangle 2
            v1 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
            v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z + 1);
            v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z + 1), z+1);
            normal = glm::cross(v1 - v2, v1 - v3);
            normals[x+1][z] += normal;
            normals[x][z+1] += normal;
            normals[x+1][z+1] += normal;
            verticies[count++] = v1;
            verticies[count++] = normal;
            count++;
            verticies[count++] = v2;
            verticies[count++] = normal;
            count++;
            verticies[count++] = v3;
            verticies[count++] = normal;
            count++;
        }
    }

    // int z = -1;
    // for (int x = 0; x < CHUNK_WIDTH-1; x++) {
    //     // triangle 1
    //     v1 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z), z);
    //     v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
    //     normal = glm::cross(v1, v3);
    //     normals[x][z+1] += normal;

    //     // triangle 2
    //     v1 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
    //     v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z + 1);
    //     normal = glm::cross(v2, v1);
    //     normals[x][z+1] += normal;
    //     normals[x+1][z+1] += normal;
    // }

    int x = -1;
    for (int z = 0; z < CHUNK_WIDTH-1; z++) {
        // triangle 1
        v1 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z), z);
        v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z+1);
        v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
        normal = glm::cross(v1 - v2, v1 - v3);
        normals[x+1][z] += normal;
        // triangle 2
        v1 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
        v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z + 1);
        v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z + 1), z+1);
        normal = glm::cross(v1 - v2, v1 - v3);
        normals[x+1][z] += normal;
        normals[x+1][z+1] += normal;
    }

    x = CHUNK_WIDTH - 1;
    for (int z = 0; z < CHUNK_WIDTH-1; z++) {
        // triangle 1
        v1 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z), z);
        v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z+1);
        v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
        normal = glm::cross(v1 - v2, v1 - v3);
        normals[x][z] += normal;
        normals[x][z+1] += normal;
        // triangle 2
        v1 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
        v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z + 1);
        v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z + 1), z+1);
        normal = glm::cross(v1 - v2, v1 - v3);
        normals[x][z+1] += normal;
    }

    int z = -1;
    for (int x = 0; x < CHUNK_WIDTH-1; x++) {
        // triangle 1
        v1 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z), z);
        v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z+1);
        v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
        normal = glm::cross(v1 - v2, v1 - v3);
        normals[x][z+1] += normal;
        // triangle 2
        v1 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
        v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z + 1);
        v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z + 1), z+1);
        normal = glm::cross(v1 - v2, v1 - v3);
        normals[x][z+1] += normal;
        normals[x+1][z+1] += normal;
    }

    z = CHUNK_WIDTH - 1;
    for (int x = 0; x < CHUNK_WIDTH-1; x++) {
        // triangle 1
        v1 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z), z);
        v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z+1);
        v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
        normal = glm::cross(v1 - v2, v1 - v3);
        normals[x][z] += normal;
        normals[x+1][z] += normal;
        // triangle 2
        v1 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z), z);
        v2 = glm::vec3(x, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x, chunkY*(CHUNK_WIDTH - 1) + z + 1), z + 1);
        v3 = glm::vec3(x+1, generator->getHeight(chunkX*(CHUNK_WIDTH - 1) + x + 1, chunkY*(CHUNK_WIDTH - 1) + z + 1), z+1);
        normal = glm::cross(v1 - v2, v1 - v3);
        normals[x+1][z] += normal;
    }

    count = 0;

    for (int x = 0; x < CHUNK_WIDTH-1; x++) {
        for (int z = 0; z < CHUNK_WIDTH-1; z++) {
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

    glGenVertexArrays(1, &_VAO);
    glBindVertexArray(_VAO);

    glGenBuffers(1, &_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, _VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*2*3*3*2*(CHUNK_WIDTH-1)*(CHUNK_WIDTH-1), verticies, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), 0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));

    if (glGetError() != GL_NO_ERROR) {
        std::cout << "broke" << std::endl;
    }

    for (int x = 0; x < CHUNK_WIDTH; x++) {
        delete[] normals[x];
    }
    delete[] normals;
    delete[] verticies;
}

glm::vec3 Chunk::getColor(float height, float normalY) {
    if (height < -5) {
        return glm::vec3(113.0f/256.0f, 110.0f/256.0f, 62.0f/256.0f);
    } else if (height < -2) {
        return glm::vec3(170.0f/256.0f, 163.0f/256.0f, 57.0f/256.0f);
    } else if (height < 1) {
        return glm::vec3(227.0f/256.0f, 215.0f/256.0f, 26.0f/256.0f);
    } else if (height <= 10 && normalY > (height/10.0f)*(height/10.0f) + 0.6f) {
        return glm::vec3(227.0f/256.0f, 215.0f/256.0f, 26.0f/256.0f);
    } else if (height < 35) {
        return glm::vec3(45.0f/256.0f, 136.0f/256.0f, 45.0f/256.0f);
    } else if (height < 50 && normalY > 0.5f) {
        return glm::vec3(45.0f/256.0f, 136.0f/256.0f, 45.0f/256.0f);
    } else if (height < 65 && normalY > 0.75f) {
        return glm::vec3(45.0f/256.0f, 136.0f/256.0f, 45.0f/256.0f);
    } else if (height < 75 && normalY > 0.9f) {
        return glm::vec3(45.0f/256.0f, 136.0f/256.0f, 45.0f/256.0f);
    } else if (height > 90 && normalY > 0.85f) {
        return glm::vec3(1, 1, 1);
    } else if (height > 100) {
        return glm::vec3(1, 1, 1);
    } else {
        return glm::vec3(0.752941, 0.772549, 0.8078431)/2.0f;
    }
}

Chunk::~Chunk() {
    glDeleteBuffers(1, &_VBO);
    glDeleteVertexArrays(1, &_VAO);
}

void Chunk::draw() {
    glBindVertexArray(_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 2*3*(CHUNK_WIDTH-1)*(CHUNK_WIDTH-1));
}