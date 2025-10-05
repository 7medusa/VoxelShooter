#pragma once
#include <vector>
#include <fstream>
#include <glm/glm.hpp>
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "defines.h"

class Camera;

class Model {
public:
    Model(Camera* camera, float rotate=0.0f, glm::vec3 set=glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 scale=glm::vec3(1.0f, 1.0f, 1.0f));

    std::vector<Vertex> vertices;
    uint64_t numVertices{};
    std::vector<uint32_t> indices;
    int64_t numIndices{};
    std::ifstream input;
    glm::mat4 model{};
    glm::mat4 modelViewProj{};
    glm::mat4 modelView{};
    glm::mat4 invModelView{};
    glm::mat4 projection{};
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
};