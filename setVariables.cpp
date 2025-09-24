#include "includes.h"
#include "camera.h"

void setVariables(glm::mat4 modelViewProj, glm::mat4 projection, glm::mat4 model, const int modelViewProjLocation, int64_t numIndices, const VertexBuffer* vertexBuffer, const IndexBuffer* indexBuffer, int modelViewLocation, int invModelViewLocation, glm::mat4 modelView, glm::mat4 invModelView, Camera* camera) {
    vertexBuffer->bind();
    indexBuffer->bind();
    projection = camera->getViewProjection();
    modelViewProj = projection * model;
    modelView = camera->getView() * model;
    invModelView = glm::transpose(glm::inverse(modelView));
    GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &modelViewProj[0][0]));//ändert die daten in der modelViewPorjLocation im shader
    GLCALL(glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, &modelView[0][0]));//für schatten
    GLCALL(glUniformMatrix4fv(invModelViewLocation, 1, GL_FALSE, &invModelView[0][0]));//für schatten
    VertexBuffer::unbind();
    IndexBuffer::unbind();
}