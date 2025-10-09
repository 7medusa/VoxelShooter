#include "projektil.h"
#include <iostream>
#include "glm/glm.hpp"
#include "model.h"
#include "mesh.h"
#include "dir.h"
#include "camera.h"

Projektil::Projektil(int damage, Shader* shader, Camera* camera, bool direction) {
    this->damage = damage;
    this->projectil = Model(camera, 0.0f, glm::vec3(0.0f, ground+1.0f, 0.0f), glm::vec3(1.0f));
    this->projectilMesh = ModelRead(projectilModelDir, shader);
    this->direction = direction;
}

Projektil::~Projektil() {
    cout << "projektil deleted" << endl;
}

void Projektil::moveRight() {}

void Projektil::moveLeft() {}