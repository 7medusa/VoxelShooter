#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(float fov, float width, float height) {
    projection = glm::perspective(fov/2.0f, width/height, 0.1f, 1000.0f);//perspektive
    view = glm::mat4(1.0f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    viewProj = projection * view;
}

Camera::~Camera() = default;

void Camera::update() {//updatet die kamera
    viewProj = projection * view;
}
void Camera::translate(glm::vec3 v) {//bewegt die kamera
    position += v;
    view = glm::translate(view, v * -1.0f);
}
glm::mat4 Camera::getViewProjection() const {
    return viewProj;
}
glm::vec3 Camera::getPosition() const {
    return position;
}
void Camera::reset() {
    float difference = position.z - 5.0f;
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, difference));
    position.z = 5.0f;
}
glm::mat4 Camera::getView() {
    return view;
}
void Camera::resetMatrix() {
    view = glm::mat4(1.0f);
}