#pragma once
#include <vector>
#include <memory>
#include "model.h"
#include "mesh.h"

using namespace std;

class Camera;
class Shader;

class Projektil {
public:
    Projektil(int damage, Shader* shader, Camera* camera, bool direction);
    ~Projektil();
    void moveRight();
    void moveLeft();
    Model projectilModel;
    ModelRead projectilMesh;
    bool direction;
private:
    int damage;
    float moveSpeed;
};

static vector<unique_ptr<Projektil>> characterProjektile;
static vector<unique_ptr<Projektil>> enemyProjektile;