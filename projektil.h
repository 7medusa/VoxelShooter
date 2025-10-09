#pragma once
#include <vector>

using namespace std;

class Model;
class ModelRead;
class Camera;
class Shader;

class Projektil {
public:
    Projektil(int damage, Shader* shader, Camera* camera, bool direction);
    ~Projektil();
    void moveRight();
    void moveLeft();
    Model projectil;
    ModelRead projectilMesh;
    bool direction;
private:
    int damage;
    float moveSpeed;
};

vector<Projektil>;