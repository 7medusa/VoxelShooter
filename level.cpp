#include "level.h"
#include <iostream>
#include "camera.h"
#include "setVariables.h"
#include "mesh.h"
#include "model.h"
#include "font.h"
#include "control.h"
#include <SDL2/SDL.h>
#include "error.h"
#include "projektil.h"
#include "time.h"
#include "character.h"

Level::Level(Camera* camera, Shader* shader, glm::mat4* characterPosition, float* delta, float* gameTime, char* levelModelDir, unsigned int* level) {
    levelModel = new Model(camera, 0, glm::vec3(11.2f, ground-0.09, 0.0f), glm::vec3(1.0f));
    levelMesh = new ModelRead(levelModelDir, shader);
    this->characterPosition = characterPosition;
    this->shader = shader;
    switch(*level) {
        case 1:
            levelEnemy.push_back(make_unique<Enemy>(characterPosition, shader, camera, glm::vec3(12.0f, ground, 0.0f), delta, gameTime, "soldier"));
            break;
        case 2:
            for(auto & i : levelEnemy) {
                i.reset();
            }
            levelEnemy.push_back(make_unique<Enemy>(characterPosition, shader, camera, glm::vec3(12.0f, ground, 0.0f), delta, gameTime, "soldier"));
            levelEnemy.push_back(make_unique<Enemy>(characterPosition, shader, camera, glm::vec3(15.0f, ground, 0.0f), delta, gameTime, "soldier"));
            break;
        default:
            for(auto & i : levelEnemy) {
                i.reset();
            }
            Error::runtimeError();
            break;
    }
}

Level::~Level() {
    delete levelModel;
    delete levelMesh;
    clog << "\033[34m" << "level1 deleted" << "\033[0m" << endl;
}

void Level::logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, Font* font, Shader* fontShader, SDL_Window* window, unsigned int* level, int windowWidth, int windowHeight, Control* control, Time time, Character* player) {
    setVariables(levelModel->modelViewProj, projection, levelModel->model, modelViewProjLocation, &levelModel->vertexBuffer, &levelModel->indexBuffer, modelViewLocation, invModelViewLocation, levelModel->modelView, levelModel->invModelView, camera);
    levelMesh->render();

    iteratorProjektile(&enemyProjektile, camera, projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, time.delta, *level, player, "player");

    for(auto it = levelEnemy.begin(); it!= levelEnemy.end(); it++) {
        auto& i = *it;
        if(i != nullptr) {
            if(i->life > 0) {
                setVariables(i->enemyModel.modelViewProj, projection, i->enemyModel.model, modelViewProjLocation, &i->enemyModel.vertexBuffer, &i->enemyModel.indexBuffer, modelViewLocation, invModelViewLocation, i->enemyModel.modelView, i->enemyModel.invModelView, camera);
                i->enemyMesh.render();
                i->followPlayer(*characterPosition, shader, camera);
                iteratorProjektile(&characterProjektile, camera, projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, time.delta, *level, player, "enemy", i.get());
            }
            else if(i->life <= 0) {
                levelEnemy.erase(it);
                iteratorProjektile(&characterProjektile, camera, projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, time.delta, *level, player, "enemy");
                return;
            }
        }
    }
    if(levelEnemy.size() == 0) {
        iteratorProjektile(&characterProjektile, camera, projection, modelViewProjLocation, modelViewLocation, invModelViewLocation, time.delta, *level, player, "enemy");
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

vector<unique_ptr<Enemy>> levelEnemy;
unique_ptr<Level> level;