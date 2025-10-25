#include "character.h"
#include "dir.h"
#include "camera.h"
#include "defines.h"
#include "projektil.h"
#include "shader.h"
#include <iostream>

Character::Character(Shader* shader, Camera* camera) :
    characterModel(camera, 0.0f, glm::vec3(0.0f, ground, 0.0f), glm::vec3(characterScale)),
    characterMesh(characterModelDir, shader) {
    damage = characterDamage;
    life = characterLife;
}

void Character::shoot(bool direction, Shader* shader, Camera* camera) {
    if(direction) {
        characterProjektile.push_back(make_unique<Projektil>(damage, shader, camera, true, characterModel.model));
    }
    else {
        characterProjektile.push_back(make_unique<Projektil>(damage, shader, camera, false, characterModel.model));
    }
}

void Character::getDamage(int damage) {
    clog << "\033[34m" << "player damage" << "\033[0m" << endl;
    life -= damage;
    if(life == 0) {
        dead();
    }
}

void Character::dead() {
    clog << "\033[34m" << "player died" << "\033[0m" << endl;
}

void Character::walkAnimation() {}