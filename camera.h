#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float fov, float width, float height) {
#ifdef orthoMode
        projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);//ortho
#else
        projection = glm::perspective(fov/2.0f, width/height, 0.1f, 1000.0f);//perspektive
#endif
        view = glm::mat4(1.0f);
        position = glm::vec3(0.0f, 0.0f, 0.0f);
        viewProj = projection * view;
    }

    virtual ~Camera() = default;

    virtual void update() {//updatet die kamera
        viewProj = projection * view;
    }
    virtual void translate(glm::vec3 v) {//bewegt die kamera
        position += v;
        view = glm::translate(view, v * -1.0f);
    }
    glm::mat4 getViewProjection() const {
        return viewProj;
    }
    glm::vec3 getPosition() const {
        return position;
    }
protected:
    glm::mat4 projection{};//matrix zur abbildung der objekte
    glm::mat4 view{};//position der kamera
    glm::mat4 viewProj{};//kombinierte matrix
    glm::vec3 position{};//position der kamera
};