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
    cout << "player gets damage: " << damage << endl;
}

void Character::walkAnimation() {}