#pragma once
#include <glm/glm.hpp>
#include <string>

using namespace std;

class Enemy {
protected:
    void followPlayer(glm::mat4 characterPosition);
    void shootPlayer(int damage);
    void getDamage(int life);
    void walkAnimation();
};

class Tank {
public:
    Tank(glm::mat4* characterPosition);
    ~Tank();
    int life;
    int damage;
    glm::mat4* position;
};

class Soldier {
public:
    Soldier(glm::mat4* characterPosition);
    ~Soldier();
    static string randomModel();
    int life;
    int damage;
    glm::mat4* position;
private:
    string model;
};