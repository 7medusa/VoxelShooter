#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "dir.h"

void Enemy::followPlayer(glm::mat4 characterPosition) {}

void Enemy::waitForPlayer(glm::mat4 characterPosition) {}

void Enemy::shootPlayer(int damage) {}

void Enemy::getDamage(int life) {}

void Enemy::walkAnimation() {}

Tank::Tank(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn) : tankModel(camera, 0.0f, spawn, glm::vec3(characterScale)),
    tankMesh(tankModelDir, shader) {
    life = 4;
    damage = 2;
    playerPosition = characterPosition;
    enemyPosition = &tankModel.model;
}

Tank::~Tank() {
    clog << "\033[34m" << "enemy died" << "\033[0m" << endl;
}

char* Soldier::randomModel() {
    srand((unsigned)time(nullptr));
    unsigned int random = rand();
    if(random % 2 == 0) {
        return enemy1ModelDir;
    }
    return enemy2ModelDir;
}

Soldier::Soldier(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn)  : soldierModel(camera, 0.0f, spawn, glm::vec3(characterScale)),
    soldierMesh(randomModel(), shader){
    life = 2;
    damage = 1;
    playerPosition = characterPosition;
    enemyPosition = &soldierModel.model;
}

Soldier::~Soldier() {
    clog << "\033[34m" << "enemy died" << "\033[0m" << endl;
}