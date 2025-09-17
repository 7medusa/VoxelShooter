#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <cassert>
#include "../defines.h"

using namespace std;

struct Position {
    float x, y, z;
};

vector<Position> positions;
vector<float> indices;

void processMesh(aiMesh* mesh, const aiScene* scene) {
    for(int i = 0; i < mesh->mNumVertices; i++) {
        Position vertex;
        vertex.x = mesh->mVertices[i].x;
        vertex.y = mesh->mVertices[i].y;
        vertex.z = mesh->mVertices[i].z;
        positions.push_back(vertex);
    }
    for(int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        assert(face.mNumIndices == 3);
        for(int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
}

void processNode(aiNode* node, const aiScene* scene) {
    for(int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for(int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

int main(int argc, char** argv) {

#ifndef Release
    cout << "argument: " << argv[1] << endl;
#endif

    if(argc <= 0) {
        return -1;
    }
    else if(argc < 2) {
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
    return 0;
}