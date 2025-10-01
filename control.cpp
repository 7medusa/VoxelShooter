#include "includes.h"

class Control {
public:
    bool wBool = false;
    bool sBool = false;
    bool aBool = false;
    bool dBool = false;
    bool jumpOnProgress = false;
    bool up = true;
    void handle(SDL_Event* event, Camera* camera) {
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
    void control(Camera* camera, glm::mat4* characterModel, glm::mat4* projection, glm::mat4* characterModelViewProj, float delta) {
        glm::vec3 characterPosition = glm::vec3((*characterModel)[3]);
        if(wBool) {
            if(camera->getPosition().z > zoomIn) {
                camera->translate(glm::vec3(0.0f, 0.0f, -zoomSpeed * delta));
            }
        }
        if(sBool) {
            if(camera->getPosition().z < zoomOut) {
                camera->translate(glm::vec3(0.0f, 0.0f, zoomSpeed * delta));
            }
        }
        if(aBool && camera->getPosition().x > borderLeft) {
            camera->translate(glm::vec3(-walkSpeed * delta, 0.0f, 0.0f));
            *characterModel = glm::mat4(1.0f);
            *characterModel = glm::translate(*characterModel, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
            *characterModel = glm::scale(*characterModel, glm::vec3(0.011f));
        }
        if(dBool && camera->getPosition().x < borderRight) {
            camera->translate(glm::vec3(walkSpeed * delta, 0.0f, 0.0f));
            *characterModel = glm::mat4(1.0f);
            *characterModel = glm::translate(*characterModel, glm::vec3(camera->getPosition().x, characterPosition.y, characterPosition.z));
            *characterModel = glm::scale(*characterModel, glm::vec3(0.011f));
        }
        if(jumpOnProgress) {
            characterPosition = glm::vec3((*characterModel)[3]);
            if(up) {
                if(characterPosition.y < jumpHeight) {
                    *characterModel = glm::translate(*characterModel, glm::vec3(0.0f, jumpSpeed * delta, 0.0f));
                }
                else {
                    up = false;
                }
            }
            else {
                if(characterPosition.y > ground) {
                    *characterModel = glm::translate(*characterModel, glm::vec3(0.0f, -jumpSpeed * delta, 0.0f));
                }
                else {
                    *characterModel = glm::mat4(1.0f);
                    *characterModel = glm::translate(*characterModel, glm::vec3(characterPosition.x, ground, characterPosition.z));
                    *characterModel = glm::scale(*characterModel, glm::vec3(0.011f));
                    up = true;
                    jumpOnProgress = false;
                }
            }
            *characterModelViewProj = *projection * *characterModel;
        }
    }
};