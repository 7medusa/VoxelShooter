#include "projektil.h"
#include <iostream>
#include "glm/glm.hpp"
#include "model.h"
#include "mesh.h"
#include "dir.h"
#include "camera.h"
#include "setVariables.h"
#include "error.h"

Projektil::Projektil(int damage, Shader* shader, Camera* camera, bool direction, glm::mat4 shooterPosition) :
    projectilModel(camera, 0.0f, glm::vec3(shooterPosition[3].x, ground+1.0f, shooterPosition[3].z), glm::vec3(0.5f)),
    projectilMesh(projectilModelDir, shader) {
    clog << "\033[34m" << "projectil created" << "\033[0m" << endl;
    this->damage = damage;
    this->direction = direction;
    moveSpeed = porjectileSpeed;
}

Projektil::~Projektil() {
    clog << "\033[34m" << "projectil deleted" << "\033[0m" << endl;
}

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

void iteratorProjektile(vector<unique_ptr<Projektil>>* vec, Camera* camera, glm::mat4 projection, int modelViewProjection, int modelViewLocation, int invModelViewLocation, float delta, unsigned int level) {
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
        if((*i)->projectilModel.model[3].x > rightBorder*1.1 || (*i)->projectilModel.model[3].x < leftBorder*1.1) {
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