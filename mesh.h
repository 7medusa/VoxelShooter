#pragma once
#include "includes.h"
#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H
#endif

struct Material {
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emissive;
    float shininess;
};

class Mesh {
public:
    Mesh(const char* filename, Material material, Shader* shader) {
        this->shader = shader;
        this->material = material;
        diffuseLocation = glGetUniformLocation(shader->getShaderId(), "u_diffuse");
        specularLocation = glGetUniformLocation(shader->getShaderId(), "u_specular");
        emissiveLocation = glGetUniformLocation(shader->getShaderId(), "u_emissive");
        shininessLocation = glGetUniformLocation(shader->getShaderId(), "u_shininess");

        vector<Vertex> vertices;
        vector<uint32_t> indices;
        uint64_t numVertices;
        uint64_t numIndices;

        ifstream input = ifstream(filename, ios::in | ios::binary);
        if(!input.is_open()) {
            cout << "fatal error in mesh constructor" << endl;
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
        vertexBuffer = new VertexBuffer(vertices.data(), vertices.size());
        indexBuffer = new IndexBuffer(indices.data(), indices.size(), sizeof(indices[0]));
        this->numIndices = static_cast<uint32_t>(indices.size());
    }

    ~Mesh() {
        delete vertexBuffer;
        delete indexBuffer;
    }

    void bindBuffers() {
        vertexBuffer->bind();
        indexBuffer->bind();
    }

    inline void render() {
        shader->bind();
        vertexBuffer->bind();
        indexBuffer->bind();
        glUniform3fv(diffuseLocation, 1, &material.diffuse[0]);
        glUniform3fv(specularLocation, 1, &material.specular[0]);
        glUniform3fv(emissiveLocation, 1, &material.emissive[0]);
        glUniform1f(shininessLocation, material.shininess);
        GLCALL(glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr));
    }

    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    Shader* shader;
    Material material;
    uint32_t numIndices;
    int diffuseLocation;
    int specularLocation;
    int emissiveLocation;
    int shininessLocation;
};