#pragma once
#include "model.h"
#include "mesh.h"
#include <string>

class Shader;
class Camera;

class Character {
public:
    Character(Shader* shader, Camera* camera);
    void shoot(bool direction, Shader* shader, Camera* camera);
    void getDamage(int damage);
    void walkAnimation();
    static string projektilName();
    int life;
    int damage;
    Model characterModel;
    ModelRead characterMesh;
};