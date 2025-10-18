#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "dir.h"

Tank::Tank(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn) : tankModel(camera, 0.0f, spawn, glm::vec3(characterScale)),
    tankMesh(tankModelDir, shader) {
    life = 4;
    damage = 2;
    playerPosition = characterPosition;
    enemyPosition = &tankModel.model;
}

bool Tank::isPlayer(glm::mat4 characterPosition) {
    if(characterPosition[3].x <= tankModel.model[3].x - soldierRange || characterPosition[3].x >= tankModel.model[3].x + soldierRange) {
        return true;
    }
    return false;
}

void Tank::followPlayer(glm::mat4 characterPosition) {}

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

bool Soldier::isPlayer(glm::mat4 characterPosition) {
    if(characterPosition[3].x <= soldierModel.model[3].x - soldierRange || characterPosition[3].x >= soldierModel.model[3].x + soldierRange) {
        return true;
    }
    return false;
}

void Soldier::followPlayer(glm::mat4 characterPosition) {
    if(isPlayer(characterPosition)) {
        if(characterPosition[3].x <= soldierModel.model[3].x) {
            if(characterPosition[3].x ) {}
        }
        else if(characterPosition[3].x > soldierModel.model[3].x) {}
    }
}

Soldier::~Soldier() {
    clog << "\033[34m" << "enemy died" << "\033[0m" << endl;
}