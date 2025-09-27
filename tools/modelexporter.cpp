#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <cassert>
#include <string>
#include <fstream>
#include <glm/glm.hpp>

using namespace std;

struct Position {
    float x, y, z;
};

struct Position2D {
    float x, y;
};

struct Material {
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emissive;
    float shininess;
    aiString diffuseMapName;
    aiString normalMapName;
};

struct MDSMaterial {
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emissive;
    float shininess;
};

struct Mesh {
    vector<Position> positions;
    vector<Position> normals;
    vector<Position2D> uvs;
    vector<uint32_t> indices;
    int materialIndex;
};

vector<Mesh> meshes;
vector<Material> materials;

void processMesh(const aiMesh* mesh, const aiScene* scene) {//const vielleicht entfernen
    Mesh m;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Position position;
        position.x = mesh->mVertices[i].x;
        position.y = mesh->mVertices[i].y;
        position.z = mesh->mVertices[i].z;
        m.positions.push_back(position);
        Position normal;
        normal.x = mesh->mNormals[i].x;
        normal.y = mesh->mNormals[i].y;
        normal.z = mesh->mNormals[i].z;
        m.normals.push_back(normal);
        Position2D uv;
        assert(mesh->mTextureCoords[0]);
        uv.x = mesh->mTextureCoords[0][i].x;
        uv.y = mesh->mTextureCoords[0][i].y;
        m.uvs.push_back(uv);
    }
    for(int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            m.indices.push_back(face.mIndices[j]); // NOLINT(*-narrowing-conversions)
        }
    }
    m.materialIndex = mesh->mMaterialIndex;
    meshes.push_back(m);
}

void processNode(const aiNode* node, const aiScene* scene) {//const vielleicht entfernen
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

char* getFilename(char* filename) {
    const int len = static_cast<int>(strlen(filename));
    char* lastSlash = filename;
    for(unsigned int i = 0; i < len; i++) {
        if(filename[i] == '/' || filename[i] == '\\') {
            lastSlash = filename + i + 1;
        }
    }
    return lastSlash;
}

void processMaterials(const aiScene* scene) {
    scene->mNumMaterials;
    for(unsigned int i = 0; i < scene->mNumMaterials; i++) {
        Material mat = {};
        aiMaterial* material = scene->mMaterials[i];

        //defaults
        mat.diffuse  = glm::vec3(1.0f, 1.0f, 1.0f);
        mat.specular = glm::vec3(0.0f, 0.0f, 0.0f);
        mat.emissive = glm::vec3(0.0f, 0.0f, 0.0f);
        mat.shininess = 0.0f;

        aiColor3D diffuse(0.0f, 0.0f, 0.0f);
        if(AI_SUCCESS != material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)) {
            //no diffuse
        }
        mat.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);

        aiColor3D specular(0.0f, 0.0f, 0.0f);
        if(AI_SUCCESS != material->Get(AI_MATKEY_COLOR_SPECULAR, specular)) {
            //no specular
        }
        mat.specular = glm::vec3(specular.r, specular.g, specular.b);

        aiColor3D emissive(0.0f, 0.0f, 0.0f);
        if(AI_SUCCESS != material->Get(AI_MATKEY_COLOR_EMISSIVE, emissive)) {
            //no diffuse
        }
        mat.emissive = glm::vec3(emissive.r, emissive.g, emissive.b);

        float shininess = 0.0f;
        if(AI_SUCCESS != material->Get(AI_MATKEY_SHININESS, shininess)) {
            //no diffuse
        }
        mat.shininess = shininess;

        float shininessStrength = 0.0f;
        if(AI_SUCCESS != material->Get(AI_MATKEY_SHININESS_STRENGTH, shininessStrength)) {
            //no diffuse
        }
        mat.specular = glm::vec3(shininessStrength);

        uint32_t numDiffuseMaps = material->GetTextureCount(aiTextureType_DIFFUSE);
        uint32_t numNormalMaps = material->GetTextureCount(aiTextureType_NORMALS);
        assert(numDiffuseMaps > 0);
        material->GetTexture(aiTextureType_DIFFUSE, 0, &mat.diffuseMapName);
        assert(numNormalMaps > 0);
        material->GetTexture(aiTextureType_NORMALS, 0, &mat.normalMapName);

        materials.push_back(mat);
    }
}

int main(int argc, char** argv) {

#ifndef Release
    cout << "argument: " << argv[1] << endl;
#endif

    if(argc <= 0) {
        return -1;
    }
    if(argc < 2) {
        cout << "usage: " << argv[0] << " modelfilename" << endl;
        return -1;
    }

    Assimp::Importer importer;
    //aiProcess_GenSmoothNormals
    const aiScene* scene = importer.ReadFile(argv[argc - 1], aiProcess_Triangulate | aiProcess_GenNormals| aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality);//erstellt eine szene aus der datei und optimiert sie mit flags
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "error loading model" << importer.GetErrorString() << endl;
        return -1;
    }

    processMaterials(scene);
    processNode(scene->mRootNode, scene);

    const string filename = string(getFilename(argv[argc - 1]));
    const string filenameWithoutExtension = filename.substr(0, filename.find_last_of('.'));
    string outputFilename = filenameWithoutExtension + ".mds";//psd für poseidon

    ofstream output(outputFilename, ios::out | ios::binary);
    cout << "writing data into mds file..." << endl;

    //materials
    uint64_t numMaterials = materials.size();
    output.write((char*)&numMaterials, sizeof(numMaterials));
    for(Material material : materials) {
        output.write((char*)&material, sizeof(MDSMaterial));
        const char* pathPrefix = "../mds/";//pfad der textur
        const uint64_t prefixLen = static_cast<uint64_t>(strlen(pathPrefix));
        //diffuse map
        uint64_t diffuseMapLength = material.diffuseMapName.length + prefixLen;//models+/länge für zahl
        output.write((char*)&diffuseMapLength, sizeof(diffuseMapLength));
        output.write(pathPrefix, prefixLen);
        output.write((char*)&material.diffuseMapName.data, material.diffuseMapName.length);

        //normal map
        uint64_t normalMapLength = material.normalMapName.length + prefixLen;//models+/länge für zahl
        output.write((char*)&normalMapLength, sizeof(normalMapLength));
        output.write(pathPrefix, 7);
        output.write((char*)&material.normalMapName.data, material.normalMapName.length);
    }

    //meshes
    int numMeshes = meshes.size();
    output.write((char*)&numMeshes, sizeof(numMeshes));
    for(Mesh& mesh : meshes) {
        uint64_t numVertices = mesh.positions.size();
        uint64_t numIndices = mesh.indices.size();
        uint64_t materialIndex = mesh.materialIndex;
        //output.write((char*)&mesh.material, sizeof(Material));
        output.write((char*)&materialIndex, sizeof(uint64_t));
        output.write((char*)&numVertices, sizeof(uint64_t));
        output.write((char*)&numIndices, sizeof(uint64_t));
        for(uint64_t i = 0; i < numVertices; i++) {
            output.write((char*)&mesh.positions[i].x, sizeof(float));
            output.write((char*)&mesh.positions[i].y, sizeof(float));
            output.write((char*)&mesh.positions[i].z, sizeof(float));
            output.write((char*)&mesh.normals[i].x, sizeof(float));
            output.write((char*)&mesh.normals[i].y, sizeof(float));
            output.write((char*)&mesh.normals[i].z, sizeof(float));
            output.write((char*)&mesh.uvs[i].x, sizeof(float));
            output.write((char*)&mesh.uvs[i].y, sizeof(float));
        }
        for(uint64_t i = 0; i < numIndices; i++) {
            output.write((char*)&mesh.indices[i], sizeof(uint32_t));
        }
    }
    output.close();
    cout << "done writing data" << endl;


    return 0;
}