#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera {
public:
    Camera(float fov, float width, float height) {
#ifdef orthoMode
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);//ortho
#else
        projection = glm::perspective(fov/2.0f, width/height, 0.1f, 1000.0f);//perspektive
#endif
        view = glm::mat4(1.0f);
        update();
    }

    glm::mat4 getViewProjection() {
        return viewProj;
    }
    void update() {//updatet die kamera
        viewProj = projection * view;
    }
    void translate(glm::vec3 v) {//bewegt die kamera
        view = glm::translate(view, v*-1.0f);
    }
private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 viewProj;
};