#pragma once
#include <glm/glm.hpp>
#include <string>
#include "mesh.h"
#include "model.h"

using namespace std;

class Tank {
public:
    Tank(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn, float* delta);
    void followPlayer(glm::mat4 characterPosition, Shader* shader, Camera* camera);
    bool isPlayer(glm::mat4 characterPosition);
    void walk(bool direction);
    void shoot(bool direction, Shader* shader, Camera* camera);
    ~Tank();
    int life;
    int damage;
    Model tankModel;
    ModelRead tankMesh;
    glm::mat4* enemyPosition;
private:
    float* delta;
    glm::mat4* playerPosition;
};

class Soldier {
public:
    Soldier(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn, float* delta);
    void followPlayer(glm::mat4 characterPosition, Shader* shader, Camera* camera);
    bool isPlayer(glm::mat4 characterPosition);
    void walk(bool direction);
    void shoot(bool direction, Shader* shader, Camera* camera);
    static char* randomModel();
    ~Soldier();
    int life;
    int damage;
    Model soldierModel;
    ModelRead soldierMesh;
    glm::mat4* enemyPosition;
private:
    float* delta;
    glm::mat4* playerPosition;
};