#include "character.h"
#include "dir.h"
#include "camera.h"
#include "defines.h"
#include "projektil.h"
#include "shader.h"

Character::Character(Shader* shader, Camera* camera) :
    characterModel(camera, 0.0f, glm::vec3(0.0f, ground, 0.0f), glm::vec3(characterScale)),
    characterMesh(characterModelDir, shader) {
    damage = 1;
    life = 3;
}

void Character::shoot(bool direction, Shader* shader, Camera* camera) {
    if(direction) {
        characterProjektile.push_back(make_unique<Projektil>(damage, shader, camera, true));
    }
    else {
        characterProjektile.push_back(make_unique<Projektil>(damage, shader, camera, false));
    }
}

void Character::getDamage(int damage) {}

void Character::walkAnimation() {}