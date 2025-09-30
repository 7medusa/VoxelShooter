#pragma once
#include "model.cpp"
#include "includes.h"
#include "mesh.h"

class level1 {
public:
    level1(Shader* shader, Camera* camera) : shop1(camera, 3.1415926536f, glm::vec3(6.0f, ground+2.6f, 2.0f), glm::vec3(0.7f, 0.7f, 0.7f)), shop1Mesh(shop1ModelDir, shader), shop2(camera, 3.1415926536f, glm::vec3(-6.0f, ground+2.4f, 2.5f), glm::vec3(0.7f, 0.7f, 0.7f)), shop2Mesh(shop2ModelDir, shader), bodenStreet(camera, 3.1415926536f, glm::vec3(0.0f, ground-0.25f, 0.0f), glm::vec3(2.0f, 1.0f, 1.0f)), bodenStreetMesh(bodenStreetModelDir, shader){
        this->camera = camera;
        this->shader = shader;
    }

    ~level1() {
        cout << "level1 destroyed" << endl;
    };

    void render(glm::mat4 projection, int modelViewProjLocation, int modelViewLocation, int invModelViewLocation, Camera* camera) {
        setVariables(shop1.modelViewProj, projection, shop1.model, modelViewProjLocation, &shop1.vertexBuffer, &shop1.indexBuffer, modelViewLocation, invModelViewLocation, shop1.modelView, shop1.invModelView, camera);
        shop1Mesh.render();
        setVariables(shop2.modelViewProj, projection, shop2.model, modelViewProjLocation, &shop2.vertexBuffer, &shop2.indexBuffer, modelViewLocation, invModelViewLocation, shop2.modelView, shop2.invModelView, camera);
        shop2Mesh.render();
        setVariables(bodenStreet.modelViewProj, projection, bodenStreet.model, modelViewProjLocation, &bodenStreet.vertexBuffer, &bodenStreet.indexBuffer, modelViewLocation, invModelViewLocation, bodenStreet.modelView, bodenStreet.invModelView, camera);
        bodenStreetMesh.render();
    }

private:
    Shader* shader;
    Camera* camera;
    Model shop1;
    ModelRead shop1Mesh;
    Model shop2;
    ModelRead shop2Mesh;
    Model bodenStreet;
    ModelRead bodenStreetMesh;
};