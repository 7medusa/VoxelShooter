#include "includes.h"

#define jumpSpeed 5.0f
#define walkSpeed 4.2f
#define zoomSpeed 5.7f

class Control {
public:
    bool wBool = false;
    bool sBool = false;
    bool aBool = false;
    bool dBool = false;
    bool jumpOnProgress = false;
    bool up = true;
    void handle(SDL_Event* event, Camera* camera) {
        static glm::vec3 characterPosition;
        if(event->type == SDL_KEYDOWN) {
            if(event->key.keysym.sym == SDLK_w) {
                wBool = true;
            }
            if(event->key.keysym.sym == SDLK_s) {
                sBool = true;
            }
            if(event->key.keysym.sym == SDLK_a) {
                aBool = true;
            }
            if(event->key.keysym.sym == SDLK_d) {
                dBool = true;
            }
            if(event->key.keysym.sym == SDLK_SPACE) {
                jumpOnProgress = true;
            }
        }
        else if(event->type == SDL_KEYUP) {
            if(event->key.keysym.sym == SDLK_w) {
                wBool = false;
            }
            if(event->key.keysym.sym == SDLK_s) {
                sBool = false;
            }
            if(event->key.keysym.sym == SDLK_a) {
                aBool = false;
            }
            if(event->key.keysym.sym == SDLK_d) {
                dBool = false;
            }
            if(event->key.keysym.sym == SDLK_x) {
                camera->reset();
            }
        }
    }
    void control(SDL_Event* event, Camera* camera, glm::mat4* characterModel, glm::mat4* projection, glm::mat4* characterModelViewProj, float delta, int borderLeft=-10, int borderRight=10) {
        static glm::vec3 characterPosition;
        if(wBool) {
            if(camera->getPosition().z > 1.7f) {
                camera->translate(glm::vec3(0.0f, 0.0f, -zoomSpeed * delta));
            }
        }
        if(sBool) {
            if(camera->getPosition().z < 13.0f) {
                camera->translate(glm::vec3(0.0f, 0.0f, zoomSpeed * delta));
            }
        }
        if(aBool && camera->getPosition().x > borderLeft) {
            camera->translate(glm::vec3(-walkSpeed * delta, 0.0f, 0.0f));
        }
        if(dBool && camera->getPosition().x < borderRight) {
            camera->translate(glm::vec3(walkSpeed * delta, 0.0f, 0.0f));
        }
        if(jumpOnProgress) {
            characterPosition = glm::vec3((*characterModel)[3]);
            if(up) {
                if(characterPosition.y < 1.2f) {
                    *characterModel = glm::translate(*characterModel, glm::vec3(0.0f, jumpSpeed * delta, 0.0f));
                }
                else {
                    up = false;
                }
            }
            else {
                if(characterPosition.y > 0.0f) {
                    *characterModel = glm::translate(*characterModel, glm::vec3(0.0f, -jumpSpeed * delta, 0.0f));
                }
                else {
                    *characterModel = glm::mat4(1.0f);
                    *characterModel = glm::translate(*characterModel, glm::vec3(characterPosition.x, 0.0f, characterPosition.x));//reseten der position auf genau 0
                    up = true;
                    jumpOnProgress = false;
                }
            }
            *characterModelViewProj = *projection * *characterModel;
        }
    }
};