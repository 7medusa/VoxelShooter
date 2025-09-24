#include "includes.h"
#include <vector>
#include <fstream>
#include "camera.h"

class Model {
public:
    Model(string modelDir, Camera* camera, float rotate=0.0f, glm::vec3 set=glm::vec3(0.0f, 0.0f, 0.0f)) : vertexBuffer(vertices.data(), numVertices), indexBuffer(indices.data(), numIndices, sizeof(indices[0])) {
        model = glm::mat4(1.0f);
        model = glm::rotate(model, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, set);
        projection = camera->getViewProjection();
        modelViewProj = projection * model;
        modelView = camera->getView() * model;
        invModelView = glm::transpose(glm::inverse(modelView));
    }

    vector<Vertex> vertices{};
    uint64_t numVertices{};
    vector<uint32_t> indices{};
    int64_t numIndices{};
    ifstream input{};
    glm::mat4 model{};
    glm::mat4 modelViewProj{};
    glm::mat4 modelView{};
    glm::mat4 invModelView{};
    glm::mat4 projection{};
    const VertexBuffer vertexBuffer;
    const IndexBuffer indexBuffer;
};