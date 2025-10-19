#include "level.h"
#include <iostream>
#include "camera.h"
#include "setVariables.h"
#include "mesh.h"
#include "model.h"
#include "dir.h"
#include "font.h"
#include "control.h"
#include <SDL2/SDL.h>

Level1::Level1(Camera* camera, Shader* shader, glm::mat4* characterPosition, float* delta)
:level1Model(camera, 0, glm::vec3(11.2f, ground-0.09, 0.0f), glm::vec3(1.0f)), level1Mesh(level1ModelDir, shader),
soldier(characterPosition, shader, camera, glm::vec3(12.0f, ground, 0.0f), delta) {
    this->characterPosition = characterPosition;
}

Level1::~Level1() {
    clog << "\033[34m" << "level1 deleted" << "\033[0m" << endl;
}


void Level1::logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, Font* font, Shader* fontShader, SDL_Window* window, unsigned int* level, int windowWidth, int windowHeight, Control* control, SDL_Event* event) {
    setVariables(level1Model.modelViewProj, projection, level1Model.model, modelViewProjLocation, &level1Model.vertexBuffer, &level1Model.indexBuffer, modelViewLocation, invModelViewLocation, level1Model.modelView, level1Model.invModelView, camera);
    level1Mesh.render();

    setVariables(soldier.soldierModel.modelViewProj, projection, soldier.soldierModel.model, modelViewProjLocation, &soldier.soldierModel.vertexBuffer, &soldier.soldierModel.indexBuffer, modelViewLocation, invModelViewLocation, soldier.soldierModel.modelView, soldier.soldierModel.invModelView, camera);
    soldier.soldierMesh.render();

    soldier.followPlayer(*characterPosition);

    //talk with NPC1
    if((*characterPosition)[3].x >= 18.5f && (*characterPosition)[3].x < 20.9f) {
        font->fontDraw(fontShader, window, font, "press e for talk", windowWidth / 2 - font->measureTextWidth("press e for talk", font->cdata) / 2, windowHeight / 2 - windowHeight / 7);
        if(control->eBool) {
            *level = 2;
            resetPosition(camera);
        }
    }
}

Level2::Level2(Camera* camera, Shader* shader, glm::mat4* characterPosition, float* delta)
:level2Model(camera, 0, glm::vec3(11.2f, ground-0.09, 0.0f), glm::vec3(1.0f)), level2Mesh(level2ModelDir, shader) {
    this->characterPosition = characterPosition;
}

Level2::~Level2() {
    clog << "\033[34m" << "level2 deleted" << "\033[0m" << endl;
}

void Level2::logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, Font* font, Shader* fontShader, SDL_Window* window, unsigned int* level, int windowWidth, int windowHeight, Control* control, SDL_Event* event) {
    setVariables(level2Model.modelViewProj, projection, level2Model.model, modelViewProjLocation, &level2Model.vertexBuffer, &level2Model.indexBuffer, modelViewLocation, invModelViewLocation, level2Model.modelView, level2Model.invModelView, camera);
    level2Mesh.render();
}

unique_ptr<Level1> level1;
unique_ptr<Level2> level2;