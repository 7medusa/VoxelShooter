#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <glm/ext/matrix_transform.hpp>
#include "dir.h"
#include "projektil.h"

char* Enemy::randomModel() {
    srand((unsigned)std::time(nullptr));
    unsigned int random = rand();
    if(random % 2 == 0) {
        return enemy1ModelDir;
    }
    return enemy2ModelDir;
}

Enemy::Enemy(glm::mat4* characterPosition, Shader* shader, Camera* camera, glm::vec3 spawn, float* delta, float* time, string enemyClass)  : enemyModel(camera, 0.0f, spawn, glm::vec3(characterScale)),
    enemyMesh(randomModel(), shader) {
    prevTimeShoot = 0;
    if(enemyClass == "soldier") {
        life = soldierLife;
        damage = soldierDamage;
    }
    else if(enemyClass == "tank") {
        life = tankLife;
        damage = tankDamage;
    }
    playerPosition = characterPosition;
    enemyPosition = &enemyModel.model;
    this->delta = delta;
    this->time = time;
}


bool Enemy::isPlayer(glm::mat4 characterPosition) {
    if(characterPosition[3].x <= enemyModel.model[3].x - soldierRange || characterPosition[3].x >= enemyModel.model[3].x + soldierRange) {
        return false;
    }
    return true;
}

void Enemy::followPlayer(glm::mat4 characterPosition, Shader* shader, Camera* camera) {
    int value = characterPosition[3].x - enemyModel.model[3].x;
    glm::vec3 prevPosition = enemyModel.model[3];
    enemyModel.model = glm::mat4(1.0f);
    enemyModel.model = glm::translate(enemyModel.model, glm::vec3(prevPosition.x, prevPosition.y, prevPosition.z));
    enemyModel.model = glm::scale(enemyModel.model, glm::vec3(characterScale));

    if(isPlayer(characterPosition)) {
        //left
        if(characterPosition[3].x <= enemyModel.model[3].x) {
            if(value < -soldierShootRange && value > -soldierRange) {
                walk(false);
            }
            else if(value >=-soldierShootRange && value < 0) {
                shoot(false, shader, camera);
            }
        }

        //right
        else if(characterPosition[3].x > enemyModel.model[3].x) {
            if(value > soldierShootRange && value < soldierRange) {
                walk(true);
            }
            else if(value <=soldierShootRange && value >= 0) {
                shoot(true, shader, camera);
            }
        }
    }
}

void Enemy::walk(bool direction) {
    glm::vec3 prevPosition = enemyModel.model[3];
    if(direction) {
        enemyModel.model = glm::mat4(1.0f);
        enemyModel.model = glm::translate(enemyModel.model, glm::vec3((walkSpeedSoldier * *delta) + prevPosition.x, prevPosition.y, prevPosition.z));
        enemyModel.model = glm::rotate(enemyModel.model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        enemyModel.model = glm::scale(enemyModel.model, glm::vec3(characterScale));
    }
    else {
        enemyModel.model = glm::mat4(1.0f);
        enemyModel.model = glm::translate(enemyModel.model, glm::vec3((-walkSpeedSoldier * *delta) + prevPosition.x, prevPosition.y, prevPosition.z));
        enemyModel.model = glm::rotate(enemyModel.model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        enemyModel.model = glm::scale(enemyModel.model, glm::vec3(characterScale));
    }
}

void Enemy::getDamage(int damage) {
    clog << "\033[34m" << "enemy damage" << "\033[0m" << endl;
    life -= damage;
}

void Enemy::shoot(bool direction, Shader* shader, Camera* camera) {
    if(*time > prevTimeShoot + pistolShootTimeSoldier) {
        if(direction) {
            enemyProjektile.push_back(make_unique<Projektil>(damage, shader, camera, true, enemyModel.model));
        }
        else {
            enemyProjektile.push_back(make_unique<Projektil>(damage, shader, camera, false, enemyModel.model));
        }
        prevTimeShoot = *time;
    }
}

void Enemy::avoidOtherEnemys() {}

Enemy::~Enemy() {
    clog << "\033[34m" << "enemy died" << "\033[0m" << endl;
}