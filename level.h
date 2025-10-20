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

class Level1 {
public:
    Level1(Camera* camera, Shader* shader, glm::mat4* characterPosition, float* delta, float* gameTime);
    ~Level1();
    void logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, Font* font, Shader* fontShader, SDL_Window* window, unsigned int* level, int windowWidth, int windowHeight, Control* control, SDL_Event* event);
    Model level1Model;
    ModelRead level1Mesh;
    glm::mat4* characterPosition;
    Soldier soldier;
    Shader* shader;
};

class Level2 {
public:
    Level2(Camera* camera, Shader* shader, glm::mat4* characterPosition, float* delta, float* gameTime);
    ~Level2();
    void logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, Font* font, Shader* fontShader, SDL_Window* window, unsigned int* level, int windowWidth, int windowHeight, Control* control, SDL_Event* event);
    Model level2Model;
    ModelRead level2Mesh;
    glm::mat4* characterPosition;
    int floor;
};

extern unique_ptr<Level1> level1;
extern unique_ptr<Level2> level2;