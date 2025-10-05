#include "level.h"
#include "setVariables.h"
#include "mesh.h"
#include "model.h"
#include "dir.h"
#include "font.h"

Level1::Level1(Camera* camera, Shader* shader)
:level1Model(camera, 0, glm::vec3(11.2f, ground-0.09, 0.0f), glm::vec3(1.0f)), level1Mesh(level1ModelDir, shader){}

void Level1::logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, unsigned int* level) {
    setVariables(level1Model.modelViewProj, projection, level1Model.model, modelViewProjLocation, &level1Model.vertexBuffer, &level1Model.indexBuffer, modelViewLocation, invModelViewLocation, level1Model.modelView, level1Model.invModelView, camera);
    level1Mesh.render();
}

Level2::Level2(Camera* camera, Shader* shader)
:level2Model(camera, 0, glm::vec3(11.2f, ground-0.09, 0.0f), glm::vec3(1.0f)), level2Mesh(level2ModelDir, shader){}

void Level2::logic(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera, unsigned int* level) {
    setVariables(level2Model.modelViewProj, projection, level2Model.model, modelViewProjLocation, &level2Model.vertexBuffer, &level2Model.indexBuffer, modelViewLocation, invModelViewLocation, level2Model.modelView, level2Model.invModelView, camera);
    level2Mesh.render();
}