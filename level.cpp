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

Level::Level(Camera* camera, Shader* shader, glm::mat4* characterPosition, float* delta, float* gameTime, char* levelModelDir) {
    levelModel = new Model(camera, 0, glm::vec3(11.2f, ground-0.09, 0.0f), glm::vec3(1.0f));
    levelMesh = new ModelRead(levelModelDir, shader);
    this->characterPosition = characterPosition;
    this->shader = shader;
}

Level::~Level() {
    delete levelModel;
    delete levelMesh;
    clog << "\033[34m" << "level1 deleted" << "\033[0m" << endl;
}

void Level::logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, Font* font, Shader* fontShader, SDL_Window* window, unsigned int* level, int windowWidth, int windowHeight, Control* control) {
    setVariables(levelModel->modelViewProj, projection, levelModel->model, modelViewProjLocation, &levelModel->vertexBuffer, &levelModel->indexBuffer, modelViewLocation, invModelViewLocation, levelModel->modelView, levelModel->invModelView, camera);
    levelMesh->render();

    if(soldier->life != 0) {
        setVariables(soldier->enemyModel.modelViewProj, projection, soldier->enemyModel.model, modelViewProjLocation, &soldier->enemyModel.vertexBuffer, &soldier->enemyModel.indexBuffer, modelViewLocation, invModelViewLocation, soldier->enemyModel.modelView, soldier->enemyModel.invModelView, camera);
        soldier->enemyMesh.render();
        soldier->followPlayer(*characterPosition, shader, camera);
    }

    //talk with NPC1
    if((*characterPosition)[3].x >= 18.5f && (*characterPosition)[3].x < 20.9f) {
        font->fontDraw(fontShader, window, font, "press e for talk", windowWidth / 2 - font->measureTextWidth("press e for talk", font->cdata) / 2, windowHeight / 2 - windowHeight / 7);
        if(control->eBool) {
            *level = 2;
            resetPosition(camera);
        }
    }
}

Enemy* Level::returnEnemy() {
    return soldier;
}

unique_ptr<Level> level;