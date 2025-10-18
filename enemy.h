#pragma once
#include <glm/glm.hpp>
#include <string>
#include "mesh.h"
#include "model.h"

using namespace std;

class Enemy {
protected:
    void followPlayer(glm::mat4 characterPosition);
    void waitForPlayer(glm::mat4 characterPosition);
    void shootPlayer(int damage);
    void getDamage(int life);
    void walkAnimation();
};

class Tank {
public:
    Tank(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn);
    ~Tank();
    void followPlayer(glm::mat4 characterPosition);
    bool isPlayer(glm::mat4 characterPosition);
    int life;
    int damage;
    Model tankModel;
    ModelRead tankMesh;
    glm::mat4* enemyPosition;
private:
    glm::mat4* playerPosition;
};

class Soldier {
public:
    Soldier(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn);
    ~Soldier();
    void followPlayer(glm::mat4 characterPosition);
    bool isPlayer(glm::mat4 characterPosition);
    static char* randomModel();
    int life;
    int damage;
    Model soldierModel;
    ModelRead soldierMesh;
    glm::mat4* enemyPosition;
private:
    glm::mat4* playerPosition;
};