#include "character.h"
#include "dir.h"
#include "camera.h"
#include "defines.h"

Character::Character(Shader* shader, Camera* camera) :
    characterModel(camera, 0.0f, glm::vec3(0.0f, ground, 0.0f), glm::vec3(characterScale)),
    characterMesh(characterModelDir, shader) {
}

void Character::shoot(int damage) {}

void Character::getDamage(int life) {}

void Character::walkAnimation() {}