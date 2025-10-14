#pragma once
#include <vector>
#include <memory>
#include "model.h"
#include "mesh.h"

using namespace std;

class Camera;
class Shader;
class Character;

class Projektil {
public:
    Projektil(int damage, Shader* shader, Camera* camera, bool direction, glm::mat4 playerPosition);
    ~Projektil();
    void move(Camera* camera, glm::mat4 projection, int modelViewProjection, int modelViewLocation, int invModelViewLocation, float delta);
    Model projectilModel;
    ModelRead projectilMesh;
    bool direction;
private:
    int damage;
    float moveSpeed;
};

void iteratorProjektile(vector<unique_ptr<Projektil>>* vec, Camera* camera, glm::mat4 projection, int modelViewProjection, int modelViewLocation, int invModelViewLocation, float delta, unsigned int level);
void killProjektile(vector<unique_ptr<Projektil>>* vec, vector<unique_ptr<Projektil>>* vec2);

extern vector<unique_ptr<Projektil>> characterProjektile;
extern vector<unique_ptr<Projektil>> enemyProjektile;