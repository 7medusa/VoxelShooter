#pragma once
#include "model.h"
#include "mesh.h"

class Shader;
class Camera;

class Character {
public:
    Character(Shader* shader, Camera* camera);
    void shoot(int damage);
    void getDamage(int life);
    void walkAnimation();
    int life;
    int damage;
    Model characterModel;
    ModelRead characterMesh;
};