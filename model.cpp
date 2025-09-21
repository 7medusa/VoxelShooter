#include "includes.h"
#include <vector>
#include <fstream>

class Model {
public:
    Model(string modelDir, float r=1.0f, float g=1.0f, float b=1.0f, float u=1.0f, float v=1.0f) {
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
    }

    vector<Vertex> vertices{};
    uint64_t numVertices{};
    vector<uint32_t> indices{};
    int64_t numIndices{};
    ifstream input{};
};