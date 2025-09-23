#include "includes.h"
#include "mesh.h"

class Character {
public:
    void draw(glm::mat4 modelViewProj, glm::mat4 projection, glm::mat4 model, const int modelViewProjLocation, int64_t numIndices, const VertexBuffer* vertexBuffer, const IndexBuffer* indexBuffer, float time, Camera* camera, Shader* shader, glm::mat4 modelView, glm::mat4 invModelView, int modelViewLocation, int invModelViewLocation) {
        vertexBuffer->bind();
        indexBuffer->bind();
        float characterYCoordinate = glm::vec3(model[3]).y;//zwischenspeichern für springen funktion
        model = glm::mat4(1.0f);//zurücksetzen der matrix um mit translate nicht zu addieren
        model = glm::translate(model, glm::vec3(camera->getPosition().x, characterYCoordinate-1, 0.0f));
        model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));
        projection = camera->getViewProjection();
        modelViewProj = projection * model;
        modelView = camera->getView() * model;
        invModelView = glm::transpose(glm::inverse(modelView));
        GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &modelViewProj[0][0]));//ändert die daten in der modelViewPorjLocation im shader
        GLCALL(glUniformMatrix4fv(modelViewLocation, 1, GL_FALSE, &modelView[0][0]));//für schatten
        GLCALL(glUniformMatrix4fv(invModelViewLocation, 1, GL_FALSE, &invModelView[0][0]));//für schatten
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
        VertexBuffer::unbind();
        IndexBuffer::unbind();
    }
};