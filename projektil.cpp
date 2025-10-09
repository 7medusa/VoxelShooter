#include "projektil.h"
#include <iostream>
#include "glm/glm.hpp"
#include "model.h"
#include "mesh.h"
#include "dir.h"
#include "camera.h"

Projektil::Projektil(int damage, Shader* shader, Camera* camera, bool direction) :
    projectilModel(camera, 0.0f, glm::vec3(0.0f, ground+1.0f, 0.0f), glm::vec3(1.0f)),
    projectilMesh(projectilModelDir, shader) {
    cout << "projektil created" << endl;
    this->damage = damage;
    this->direction = direction;
    moveSpeed = porjectilSpeed;
}

Projektil::~Projektil() {
    cout << "projektil deleted" << endl;
}

void Projektil::moveRight() {}