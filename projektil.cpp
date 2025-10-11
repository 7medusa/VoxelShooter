#include "projektil.h"
#include <iostream>
#include "glm/glm.hpp"
#include "model.h"
#include "mesh.h"
#include "dir.h"
#include "camera.h"
#include "setVariables.h"

Projektil::Projektil(int damage, Shader* shader, Camera* camera, bool direction, glm::mat4 playerPosition) :
    projectilModel(camera, 0.0f, glm::vec3(playerPosition[3].x, ground+1.0f, playerPosition[3].z), glm::vec3(0.5f)),
    projectilMesh(projectilModelDir, shader) {
    cout << "projektil created" << endl;
    this->damage = damage;
    this->direction = direction;
    moveSpeed = porjectileSpeed;
}

Projektil::~Projektil() {
    cout << "projektil deleted" << endl;
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

void iteratorProjektile(vector<unique_ptr<Projektil>>* vec, Camera* camera, glm::mat4 projection, int modelViewProjection, int modelViewLocation, int invModelViewLocation, float delta) {
    for(auto& i : *vec) {
        i->move(camera, projection, modelViewProjection, modelViewLocation, invModelViewLocation, delta);
    }
}

vector<unique_ptr<Projektil>> characterProjektile;
vector<unique_ptr<Projektil>> enemyProjektile;