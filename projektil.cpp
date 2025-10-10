#include "projektil.h"
#include <iostream>
#include "glm/glm.hpp"
#include "model.h"
#include "mesh.h"
#include "dir.h"
#include "camera.h"
#include "setVariables.h"

Projektil::Projektil(int damage, Shader* shader, Camera* camera, bool direction) :
    projectilModel(camera, 0.0f, glm::vec3(0.0f, ground+1.0f, 0.0f), glm::vec3(1.0f)),
    projectilMesh(projectilModelDir, shader) {
    cout << "projektil created" << endl;
    this->damage = damage;
    this->direction = direction;
    moveSpeed = porjectileSpeed;
}

Projektil::~Projektil() {
    cout << "projektil deleted" << endl;
}

void Projektil::move(Camera* camera, glm::mat4 projection, int modelViewProjection, int modelViewLocation, int invModelViewLocation) {
    setVariables(projectilModel.modelViewProj, projection, projectilModel.model, modelViewProjection, &projectilModel.vertexBuffer,
        &projectilModel.indexBuffer, modelViewLocation, invModelViewLocation, projectilModel.modelView, projectilModel.invModelView, camera);
    if(direction) {//rechts
        projectilModel.model = glm::mat4(1.0f);
        projectilModel.model = glm::translate(projectilModel.model, glm::vec3(moveSpeed, ground + 1.0f, 0.0f));
        projectilMesh.render();
    }
    else {//links
        projectilModel.model = glm::mat4(1.0f);
        projectilModel.model = glm::translate(projectilModel.model, glm::vec3(-moveSpeed, ground + 1.0f, 0.0f));
        projectilMesh.render();
    }
}

void iteratorProjektile(vector<unique_ptr<Projektil>>* vec, Camera* camera, glm::mat4 projection, int modelViewProjection, int modelViewLocation, int invModelViewLocation) {
    for(auto& i : *vec) {
        i->move(camera, projection, modelViewProjection, modelViewLocation, invModelViewLocation);
    }
}

vector<unique_ptr<Projektil>> characterProjektile;
vector<unique_ptr<Projektil>> enemyProjektile;