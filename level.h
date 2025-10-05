#pragma once
#include <glm/glm.hpp>
#include "mesh.h"
#include "model.h"

using namespace std;

class Mesh;
class Camera;

class Level1 {
public:
    Level1(Camera* camera, Shader* shader);
    void logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, unsigned int* level);
    Model level1Model;
    ModelRead level1Mesh;
};

class Level2 {
public:
    Level2(Camera* camera, Shader* shader);
    void logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, unsigned int* level);
    Model level2Model;
    ModelRead level2Mesh;
};