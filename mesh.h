#pragma once
#include "includes.h"
#include "setVariables.cpp"
#include "libs/stb_image.h"
#ifndef OPENGL_MESH_H
#define OPENGL_MESH_H
#endif

struct MDSMaterial {
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emissive;
    float shininess;
};

struct Material {
    MDSMaterial material;
    GLuint diffuseMap;
    GLuint normalMap;
};

class Mesh {
public:
    Mesh(vector<Vertex>& vertices, vector<uint32_t>& indices, Material material, Shader* shader) {
        this->shader = shader;
        this->material = material;
        this->numIndices = numIndices;
        diffuseLocation = glGetUniformLocation(shader->getShaderId(), "u_diffuse");
        specularLocation = glGetUniformLocation(shader->getShaderId(), "u_specular");
        emissiveLocation = glGetUniformLocation(shader->getShaderId(), "u_emissive");
        shininessLocation = glGetUniformLocation(shader->getShaderId(), "u_shininess");
        diffuseMapLocation = glGetUniformLocation(shader->getShaderId(), "u_diffuse_map");

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

    void render() {
        shader->bind();
        vertexBuffer->bind();
        indexBuffer->bind();
        glUniform3fv(diffuseLocation, 1, &material.material.diffuse[0]);
        glUniform3fv(specularLocation, 1, &material.material.specular[0]);
        glUniform3fv(emissiveLocation, 1, &material.material.emissive[0]);
        glUniform1f(shininessLocation, material.material.shininess);
        glActiveTexture(GL_TEXTURE0);
        GLCALL(glBindTexture(GL_TEXTURE_2D, material.diffuseMap));
        GLCALL(glUniform1i(diffuseMapLocation, 0));
        GLCALL(glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr));
    }

    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    Shader* shader;
    uint32_t numIndices;
    int diffuseLocation;
    int specularLocation;
    int emissiveLocation;
    int shininessLocation;
    int diffuseMapLocation;
    Material material;
};

class ModelRead {
public:
    ModelRead(const char* filename, Shader* shader) {
        ifstream input = ifstream(filename, ios::in | ios::binary);
        int numMeshes = 0;
        uint64_t numMaterials = 0;

        //materials
        input.read((char*)&numMaterials, sizeof(uint64_t));
        for(unsigned int i = 0; i < numMaterials; i++) {
            Material material = {};
            input.read((char*)&material, sizeof(MDSMaterial));

            uint64_t diffuseMapLength = 0;
            input.read((char*)&diffuseMapLength, sizeof(uint64_t));
            string diffuseMapName(diffuseMapLength, '\0');
            input.read((char*)&diffuseMapName[0], diffuseMapLength);

            uint64_t normalMapLength = 0;
            input.read((char*)&normalMapLength, sizeof(uint64_t));
            string normalMapName(normalMapLength, '\0');
            input.read((char*)&normalMapName[0], normalMapLength);
            assert(diffuseMapLength > 0);
            //assert(normalMapLength > 0);

            int textureWidth = 0;
            int textureHeight = 0;
            int bitsPerPixel = 0;
            GLCALL(glGenTextures(2, &material.diffuseMap));
            stbi_set_flip_vertically_on_load(true);

            {
                auto textureBuffer = stbi_load(diffuseMapName.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);
                //assert(textureBuffer);
                assert(material.diffuseMap);

                GLCALL(glBindTexture(GL_TEXTURE_2D, material.diffuseMap));

                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

                GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
                GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB8_ALPHA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));
                GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));

                if(textureBuffer) {
                    stbi_image_free(textureBuffer);
                }
            }

            {
                auto textureBuffer = stbi_load(normalMapName.c_str(), &textureWidth, &textureHeight, &bitsPerPixel, 4);
                //assert(textureBuffer);
                //assert(material.normalMap);

                GLCALL(glBindTexture(GL_TEXTURE_2D, material.normalMap));

                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

                GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
                GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));
                GLCALL(glGenerateMipmap(GL_TEXTURE_2D));
                GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
                GLCALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 4));

                if(textureBuffer) {
                    stbi_image_free(textureBuffer);
                }
                GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
                materials.push_back(material);
            }
        }

        //mesh
        input.read((char*)&numMeshes, sizeof(int));
        for(unsigned int i = 0; i < numMeshes; i++) {
            vector<Vertex> vertices;
            vector<uint32_t> indices;
            uint64_t numVertices = 0;
            uint64_t numIndices = 0;
            uint64_t materialIndex = 0;
            input.read((char*)&materialIndex, sizeof(uint64_t));
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
                input.read((char*)&vertex.textureCoord.x, sizeof(float));
                input.read((char*)&vertex.textureCoord.y, sizeof(float));

                vertices.push_back(vertex);
            }
            for(uint64_t i = 0; i < numIndices; i++) {
                uint32_t index;
                input.read((char*)&index, sizeof(uint32_t));
                indices.push_back(index);
            }
            Mesh* mesh = new Mesh(vertices, indices, materials[materialIndex], shader);
            meshes.push_back(mesh);
        }
    }
    ~ModelRead() {
        for(Mesh* mesh : meshes) {
            delete mesh;
        }
    }

    void render() {
        for(Mesh* mesh : meshes) {
            mesh->render();
        }
    }

    vector<Mesh*> meshes;
    vector<Material> materials;
};