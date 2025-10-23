#pragma once
#include <glm/glm.hpp>
#include "mesh.h"
#include "model.h"
#include <SDL2/SDL.h>
#include <memory>
#include "enemy.h"

using namespace std;

class Mesh;
class Camera;
class Font;
class SDL_Window;
class Control;

class Level {
public:
    Level(Camera* camera, Shader* shader, glm::mat4* characterPosition, float* delta, float* gameTime, char* levelModelDir);
    ~Level();
    void logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, Font* font, Shader* fontShader, SDL_Window* window, unsigned int* level, int windowWidth, int windowHeight, Control* control);
    Enemy* returnEnemy();
    Model* levelModel{};
    ModelRead* levelMesh{};
    glm::mat4* characterPosition;
    Enemy* soldier{};
    Shader* shader;
};

extern unique_ptr<Level> level;