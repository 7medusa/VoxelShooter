#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <cassert>
#include <string>
#include <fstream>

using namespace std;

struct Position {
    float x, y, z;
};

vector<Position> positions;
vector<uint32_t> indices;

void processMesh(const aiMesh* mesh, const aiScene* scene) {//const vielleicht entfernen
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Position vertex{};
        vertex.x = mesh->mVertices[i].x;
        vertex.y = mesh->mVertices[i].y;
        vertex.z = mesh->mVertices[i].z;
        positions.push_back(vertex);
    }
    for(int i = 0; i < mesh->mNumFaces; i++) {
        const aiFace face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]); // NOLINT(*-narrowing-conversions)
        }
    }
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
    const aiScene* scene = importer.ReadFile(argv[argc - 1], aiProcess_Triangulate | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_JoinIdenticalVertices | aiProcess_ImproveCacheLocality);//erstellt eine szene aus der datei und optimiert sie mit flags
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        cout << "error loading model" << importer.GetErrorString() << endl;
        return -1;
    }

    processNode(scene->mRootNode, scene);

    const string filename = string(getFilename(argv[argc - 1]));
    const string filenameWithoutExtension = filename.substr(0, filename.find_last_of('.'));
    string outputFilename = filenameWithoutExtension + ".mds";//psd für poseidon

    ofstream output(outputFilename, ios::out | ios::binary);
    cout << "writing data into mds file..." << endl;
    uint64_t numVertices = positions.size();
    uint64_t numIndices = indices.size();
    output.write((char*)&numVertices, sizeof(numVertices));
    output.write((char*)&numIndices, sizeof(numIndices));
    for(uint64_t i = 0; i < numVertices; i++) {
        output.write((char*)&positions[i].x, sizeof(float));
        output.write((char*)&positions[i].y, sizeof(float));
        output.write((char*)&positions[i].z, sizeof(float));
    }
    for(uint64_t i = 0; i < numIndices; i++) {
        output.write((char*)&indices[i], sizeof(uint32_t));
    }
    output.close();
    cout << "done writing data" << endl;


    return 0;
}