#include "includes.h"
#include <vector>
#include <fstream>
#include "camera.h"

class Model {
public:
    Model(string modelDir, Camera* camera, float rotate=0.0f, glm::vec3 set=glm::vec3(0.0f, 0.0f, 0.0f)) {
        input = ifstream(modelDir, ios::in | ios::binary);
        if(!input.is_open()) {
            cout << "fatal error in model loading" << endl;
            return;
        }
        input.read((char*)&numVertices, sizeof(uint64_t));
        input.read((char*)&numIndices, sizeof(uint64_t));

        for(uint64_t i = 0; i < numVertices; i++) {
            Vertex vertex{};
            input.read((char*)&vertex.position.x, sizeof(float));
            input.read((char*)&vertex.position.y, sizeof(float));
            input.read((char*)&vertex.position.z, sizeof(float));
            input.read((char*)&vertex.normal.x, sizeof(float));
            input.read((char*)&vertex.normal.y, sizeof(float));
            input.read((char*)&vertex.normal.z, sizeof(float));
            vertices.push_back(vertex);
        }
        for(uint64_t i = 0; i < numIndices; i++) {
            uint32_t index;
            input.read((char*)&index, sizeof(uint32_t));
            indices.push_back(index);
        }

        projection = camera->getViewProjection();
        model = glm::mat4(1.0f);
        modelViewProj = projection * model;
        modelView = camera->getView() * model;
        invModelView = glm::transpose(glm::inverse(modelView));

        model = glm::rotate(model, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, set);
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
};