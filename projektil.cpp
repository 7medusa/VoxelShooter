#include "projektil.h"
#include "character.h"
#include "glm/glm.hpp"
#include "model.h"
#include "mesh.h"
#include "dir.h"
#include "camera.h"
#include "setVariables.h"
#include "error.h"
#include "enemy.h"
#include <iostream>

Projektil::Projektil(int damage, Shader* shader, Camera* camera, bool direction, glm::mat4 shooterPosition) :
    projectilModel(camera, 0.0f, glm::vec3(shooterPosition[3].x, ground+1.0f, shooterPosition[3].z), glm::vec3(0.5f)),
    projectilMesh(projectilModelDir, shader) {
    this->damage = damage;
    this->direction = direction;
    moveSpeed = porjectileSpeed;
}

Projektil::~Projektil() {}

void Projektil::move(Camera* camera, glm::mat4 projection, int modelViewProjection, int modelViewLocation, int invModelViewLocation, float delta) {
    if(direction) {//rechts
        projectilModel.model = glm::translate(projectilModel.model, glm::vec3(moveSpeed * delta, 0.0f, 0.0f));
        setVariables(projectilModel.modelViewProj, projection, projectilModel.model, modelViewProjection, &projectilModel.vertexBuffer,
        &projectilModel.indexBuffer, modelViewLocation, invModelViewLocation, projectilModel.modelView, projectilModel.invModelView, camera);
        projectilMesh.render();
    }
    else {//links
        projectilModel.model = glm::translate(projectilModel.model, glm::vec3(-moveSpeed * delta, 0.0f, 0.0f));
        setVariables(projectilModel.modelViewProj, projection, projectilModel.model, modelViewProjection, &projectilModel.vertexBuffer,
        &projectilModel.indexBuffer, modelViewLocation, invModelViewLocation, projectilModel.modelView, projectilModel.invModelView, camera);
        projectilMesh.render();
    }
}

void iteratorProjektile(vector<unique_ptr<Projektil>>* vec, Camera* camera, glm::mat4 projection, int modelViewProjection, int modelViewLocation, int invModelViewLocation, float delta, unsigned int level, Character* player, const string target, Enemy* enemy) {
    float targetPosition;
    if(target == "enemy" && enemy != nullptr) {
        targetPosition = enemy->enemyModel.model[3].x;
    }
    else if(target == "enemy" && enemy == nullptr) {
        targetPosition = 1000;
    }
    else if(target == "player") {
        targetPosition = player->characterModel.model[3].x;
    }
    else {
        targetPosition = 0;
    }
    float rightBorder = 0.0f;
    float leftBorder = 0.0f;
    switch(level) {
        case 1:
            rightBorder = borderRightLevel1;
            leftBorder = borderLeftLevel1;
            break;
        case 2:
            rightBorder = borderRightLevel1;
            leftBorder = borderLeftLevel1;
            break;
        default:
            Error::runtimeError();
            break;
    }
    for(auto i = vec->begin(); i != vec->end();) {
        if((*i)->projectilModel.model[3].x > rightBorder*1.5 || (*i)->projectilModel.model[3].x < leftBorder*1.5) {
            i = vec->erase(i);
        }
        else if((*i)->projectilModel.model[3].x > targetPosition - 0.3 && (*i)->projectilModel.model[3].x < targetPosition + 0.3) {
            if(target == "enemy") {
                enemy->getDamage(characterDamage);
            }
            else if(target == "player") {
                Projektil* ptr = (*i).get();
                player->getDamage(ptr->damage);
            }
            i = vec->erase(i);
        }
        else {
            (*i)->move(camera, projection, modelViewProjection, modelViewLocation, invModelViewLocation, delta);
            ++i;
        }
    }
}

void killProjektile(vector<unique_ptr<Projektil>>* vec, vector<unique_ptr<Projektil>>* vec2) {
    for(auto& i : *vec) {
        i.reset();
    }
    for(auto& i : *vec2) {
        i.reset();
    }
    vec->clear();
    vec2->clear();
}

vector<unique_ptr<Projektil>> characterProjektile;
vector<unique_ptr<Projektil>> enemyProjektile;