#pragma once
#include "defines.h"
#include "vertex_buffer.h"
#include "shader.h"
#include "index_buffer.h"
#include <GL/gl.h>
#include <vector>

using namespace std;

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
    Mesh(vector<Vertex>& vertices, vector<uint32_t>& indices, Material material, Shader* shader);
    ~Mesh();
    void bindBuffers();
    void render();

private:
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
    ModelRead(const char* filename, Shader* shader);
    ~ModelRead();
    void render();

private:
    vector<Mesh*> meshes;
    vector<Material> materials;
};