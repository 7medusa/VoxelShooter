#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "dir.h"

void Enemy::followPlayer(glm::mat4 characterPosition) {}

void Enemy::shootPlayer(int damage) {}

void Enemy::getDamage(int life) {}

void Enemy::walkAnimation() {}

Tank::Tank(glm::mat4* characterPosition, Shader* shader, Camera* camera) : tankModel(camera, 0.0f, glm::vec3(0.0f, ground, 0.0f), glm::vec3(characterScale)),
    tankMesh(tankModelDir, shader) {
    life = 4;
    damage = 2;
    playerPosition = characterPosition;
    enemyPosition = &tankModel.model;
}

Tank::~Tank() {
    clog << "tank deleted" << endl;
}

char* Soldier::randomModel() {
    srand((unsigned)time(nullptr));
    unsigned int random = rand();
    if(random % 2 == 0) {
        return enemy1ModelDir;
    }
    return enemy2ModelDir;
}

Soldier::Soldier(glm::mat4* characterPosition, Shader* shader, Camera* camera)  : soldierModel(camera, 0.0f, glm::vec3(0.0f, ground, 0.0f), glm::vec3(characterScale)),
    soldierMesh(randomModel(), shader){
    life = 2;
    damage = 1;
    playerPosition = characterPosition;
    enemyPosition = &soldierModel.model;
}

Soldier::~Soldier() {}