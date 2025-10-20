#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glm/ext/matrix_transform.hpp>
#include "dir.h"
#include "projektil.h"

Tank::Tank(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn, float* delta) : tankModel(camera, 0.0f, spawn, glm::vec3(characterScale)),
    tankMesh(tankModelDir, shader) {
    life = tankLife;
    damage = tankDamage;
    playerPosition = characterPosition;
    enemyPosition = &tankModel.model;
    this->delta = delta;
}

bool Tank::isPlayer(glm::mat4 characterPosition) {
    if(characterPosition[3].x <= tankModel.model[3].x - soldierRange || characterPosition[3].x >= tankModel.model[3].x + soldierRange) {
        return false;
    }
    return true;
}

void Tank::followPlayer(glm::mat4 characterPosition) {
    int value = characterPosition[3].x - tankModel.model[3].x;
    glm::vec3 prevPosition = tankModel.model[3];
    tankModel.model = glm::mat4(1.0f);
    tankModel.model = glm::translate(tankModel.model, glm::vec3(prevPosition.x, prevPosition.y, prevPosition.z));
    tankModel.model = glm::scale(tankModel.model, glm::vec3(characterScale));
    cout << value << endl;

    if(isPlayer(characterPosition)) {
        //left
        if(characterPosition[3].x <= tankModel.model[3].x) {
            if(value < -soldierShootRange && value > -soldierRange) {
                walk(false);
            }
            else if(value >=-soldierShootRange && value < 0) {
                shoot(false);
            }
        }

        //right
        else if(characterPosition[3].x > tankModel.model[3].x) {
            if(value > soldierShootRange && value < soldierRange) {
                walk(true);
            }
            else if(value <=soldierShootRange && value >= 0) {
                shoot(true);
            }
        }
    }
}

void Tank::walk(bool direction) {
    glm::vec3 prevPosition = tankModel.model[3];
    if(direction) {
        tankModel.model = glm::mat4(1.0f);
        tankModel.model = glm::translate(tankModel.model, glm::vec3((walkSpeedSoldier * *delta) + prevPosition.x, prevPosition.y, prevPosition.z));
        tankModel.model = glm::rotate(tankModel.model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        tankModel.model = glm::scale(tankModel.model, glm::vec3(characterScale));
    }
    else {
        tankModel.model = glm::mat4(1.0f);
        tankModel.model = glm::translate(tankModel.model, glm::vec3((-walkSpeedSoldier * *delta) + prevPosition.x, prevPosition.y, prevPosition.z));
        tankModel.model = glm::rotate(tankModel.model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        tankModel.model = glm::scale(tankModel.model, glm::vec3(characterScale));
    }
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

Soldier::Soldier(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn, float* delta)  : soldierModel(camera, 0.0f, spawn, glm::vec3(characterScale)),
    soldierMesh(randomModel(), shader){
    life = soldierLife;
    damage = soldierDamage;
    playerPosition = characterPosition;
    enemyPosition = &soldierModel.model;
    this->delta = delta;
}

bool Soldier::isPlayer(glm::mat4 characterPosition) {
    if(characterPosition[3].x <= soldierModel.model[3].x - soldierRange || characterPosition[3].x >= soldierModel.model[3].x + soldierRange) {
        return false;
    }
    return true;
}

void Soldier::followPlayer(glm::mat4 characterPosition) {
    int value = characterPosition[3].x - soldierModel.model[3].x;
    glm::vec3 prevPosition = soldierModel.model[3];
    soldierModel.model = glm::mat4(1.0f);
    soldierModel.model = glm::translate(soldierModel.model, glm::vec3(prevPosition.x, prevPosition.y, prevPosition.z));
    soldierModel.model = glm::scale(soldierModel.model, glm::vec3(characterScale));
    cout << value << endl;

    if(isPlayer(characterPosition)) {
        //left
        if(characterPosition[3].x <= soldierModel.model[3].x) {
            if(value < -soldierShootRange && value > -soldierRange) {
                walk(false);
            }
            else if(value >=-soldierShootRange && value < 0) {
                shoot(false);
            }
        }

        //right
        else if(characterPosition[3].x > soldierModel.model[3].x) {
            if(value > soldierShootRange && value < soldierRange) {
                walk(true);
            }
            else if(value <=soldierShootRange && value >= 0) {
                shoot(true);
            }
        }
    }
}

void Soldier::walk(bool direction) {
    glm::vec3 prevPosition = soldierModel.model[3];
    if(direction) {
        soldierModel.model = glm::mat4(1.0f);
        soldierModel.model = glm::translate(soldierModel.model, glm::vec3((walkSpeedSoldier * *delta) + prevPosition.x, prevPosition.y, prevPosition.z));
        soldierModel.model = glm::rotate(soldierModel.model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        soldierModel.model = glm::scale(soldierModel.model, glm::vec3(characterScale));
    }
    else {
        soldierModel.model = glm::mat4(1.0f);
        soldierModel.model = glm::translate(soldierModel.model, glm::vec3((-walkSpeedSoldier * *delta) + prevPosition.x, prevPosition.y, prevPosition.z));
        soldierModel.model = glm::rotate(soldierModel.model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        soldierModel.model = glm::scale(soldierModel.model, glm::vec3(characterScale));
    }
}

void Soldier::shoot(bool direction, Shader* shader, Camera* camera) {
    if(direction) {
        enemyProjektile.push_back(make_unique<Projektil>(damage, shader, camera, true, soldierModel.model));
    }
    else {
        enemyProjektile.push_back(make_unique<Projektil>(damage, shader, camera, false, soldierModel.model));
    }
}

Soldier::~Soldier() {
    clog << "\033[34m" << "enemy died" << "\033[0m" << endl;
}