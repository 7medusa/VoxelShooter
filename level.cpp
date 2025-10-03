#pragma once
#include "defines.h"
#include "includes.h"
#include "mesh.h"
#include "model.cpp"
#include "camera.h"
#include "shader.h"
#include "setVariables.cpp"

//level1
void level1Door(SDL_Event* event, float cameraPosition, unsigned int* level, Camera* camera, Shader* shader, glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation) {
    if(cameraPosition < 1 && cameraPosition > 1) {
        Model e(camera, 0.0f, glm::vec3(-19.175f, ground+2.35, -1.0f), glm::vec3(0.1f));
        ModelRead eMesh(textPressEDir, shader);
        setVariables(e.modelViewProj, projection, e.model, modelViewProjLocation, &e.vertexBuffer, &e.indexBuffer, modelViewLocation, invModelViewLocation, e.modelView, e.invModelView, camera);
        eMesh.render();
        if(event->key.keysym.sym == SDLK_e) {
            *level = 2;
        }
    }
}