#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "dir.h"

void Enemy::followPlayer(glm::mat4 characterPosition) {}

void Enemy::shootPlayer(int damage) {}

void Enemy::getDamage(int life) {}

void Enemy::walkAnimation() {}

Tank::Tank(glm::mat4* characterPosition) {
    life = 4;
    damage = 2;
    position = characterPosition;
}

Tank::~Tank() {
    cout << "tank deleted" << endl;
}

Soldier::Soldier(glm::mat4* characterPosition) {
    life = 2;
    damage = 1;
    model = randomModel();
    position = characterPosition;
}

Soldier::~Soldier() {
    cout << "soldier deleted" << endl;
}

string Soldier::randomModel() {
    srand((unsigned)time(nullptr));
    if(rand() % 2 == 0) {
        return enemy1ModelDir;
    }
    else {
        return enemy2ModelDir;
    }
}