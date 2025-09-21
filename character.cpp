#include "includes.h"

class Character {
public:
    Character(int lifeLoad=1, int moneyLoad=0) {
        life = lifeLoad;
    }

    void draw(glm::mat4 modelViewProj, glm::mat4 projection, glm::mat4 model, const int modelViewProjLocation, int64_t numIndices, const VertexBuffer* vertexBuffer, const IndexBuffer* indexBuffer, float time, Camera* camera) {
        vertexBuffer->bind();
        indexBuffer->bind();
        float characterYCoordinate = glm::vec3(model[3]).y;//zwischenspeichern für springen funktion
        model = glm::mat4(1.0f);//zurücksetzen der matrix um mit translate nicht zu addieren
        model = glm::translate(model, glm::vec3(camera->getPosition().x, characterYCoordinate-1, 0.0f));
        model = glm::rotate(model, time, glm::vec3(0.0f, 1.0f, 0.0f));
        modelViewProj = projection * model;
        GLCALL(glUniformMatrix4fv(modelViewProjLocation, 1, GL_FALSE, &modelViewProj[0][0]));//ändert die daten in der modelViewPorjLocation im shader
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
        VertexBuffer::unbind();
        IndexBuffer::unbind();
    }
    int life;
};