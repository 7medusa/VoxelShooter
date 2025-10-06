#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "defines.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "camera.h"

void setVariables(glm::mat4 modelViewProj, glm::mat4 projection, glm::mat4 model, int modelViewProjLocation, const VertexBuffer* vertexBuffer, const IndexBuffer* indexBuffer, int modelViewLocation, int invModelViewLocation, glm::mat4 modelView, glm::mat4 invModelView, Camera* camera, float time) {
    vertexBuffer->bind();
    indexBuffer->bind();
    model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));
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

void resetPosition(Camera* camera) {
    camera->translate(glm::vec3(-camera->getPosition().x, 0.0f, 0.0f));
}