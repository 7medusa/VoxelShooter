#pragma once
#include <vector>
#include <memory>
#include "model.h"
#include "mesh.h"

#include "iostream"

using namespace std;

class Camera;
class Shader;

class Projektil {
public:
    Projektil(int damage, Shader* shader, Camera* camera, bool direction);
    ~Projektil();
    void move(Camera* camera, glm::mat4 projection, int modelViewProjection, int modelViewLocation, int invModelViewLocation);
    Model projectilModel;
    ModelRead projectilMesh;
    bool direction;
private:
    int damage;
    float moveSpeed;
};

void iteratorProjektile(vector<unique_ptr<Projektil>>* vec, Camera* camera, glm::mat4 projection, int modelViewProjection, int modelViewLocation, int invModelViewLocation);

extern vector<unique_ptr<Projektil>> characterProjektile;
extern vector<unique_ptr<Projektil>> enemyProjektile;

//i->move(camera, projection, modelViewProjection, modelViewLocation, invModelViewLocation);