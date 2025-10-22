#pragma once
#include <glm/glm.hpp>
#include <string>
#include "mesh.h"
#include "model.h"

using namespace std;

class Enemy {
public:
    Enemy(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn, float* delta, float* time, string enemyClass);
    void followPlayer(glm::mat4 characterPosition, Shader* shader, Camera* camera);
    bool isPlayer(glm::mat4 characterPosition);
    void walk(bool direction);
    void getDamage(int damage);
    void shoot(bool direction, Shader* shader, Camera* camera);
    static char* randomModel();
    ~Enemy();
    int life;
    int damage;
    Model enemyModel;
    ModelRead enemyMesh;
    glm::mat4* enemyPosition;
private:
    float* time;
    float prevTimeShoot;
    float* delta;
    glm::mat4* playerPosition;
};