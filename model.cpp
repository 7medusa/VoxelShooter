#include "model.h"
#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

Model::Model(Camera* camera, float rotate, glm::vec3 set, glm::vec3 scale) : vertexBuffer(vertices.data(), numVertices), indexBuffer(indices.data(), numIndices, sizeof(indices[0])) {
    model = glm::mat4(1.0f);
    model = glm::rotate(model, rotate, glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, set);
    model = glm::scale(model, scale);
    projection = camera->getViewProjection();
    modelViewProj = projection * model;
    modelView = camera->getView() * model;
    invModelView = glm::transpose(glm::inverse(modelView));
}