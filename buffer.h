#ifndef BUFFER_H
#define BUFFER_H
#endif

/*
find . -name '*.cpp' | xargs wc -l
*/

//textur in main.cpp
/*
int textureWidth;
int textureHeight;
int bitsPerPixel;
stbi_set_flip_vertically_on_load(true);//flippt das koordinatensystem da opengl ein anderes system nutzt
const auto textureBuffer = stbi_load(textureDir, &textureWidth, &textureHeight, &bitsPerPixel, 4);//lädt die textur
GLuint textureId;
GLCALL(glGenTextures(1, &textureId));//erstellt ein texture id mit 1 textur
GLCALL(glBindTexture(GL_TEXTURE_2D, textureId));
GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));//filtert die größe der textur
GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));//setzt die eigenschaft die am rand der textur passieren soll
GLCALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
GLCALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer));//schreibt die textur in opengl
GLCALL(glBindTexture(GL_TEXTURE_2D, 0));
if(textureBuffer) {
    stbi_image_free(textureBuffer);//gibt den speicher wieder frei
}
*/

/*
Model shop1(shop1ModelDir, &camera, 3.1415926536f, glm::vec3(0.0f, -1.0f, 2.8f));
ModelRead shop1Mesh(shop1ModelDir, &shader);
Model boden(bodenModelDir, &camera, 0.0f, glm::vec3(0.0f, -3.0f, 0.0f));
ModelRead bodenMesh(bodenModelDir, &shader);
setVariables(shop1.modelViewProj, projection, shop1.model, modelViewProjLocation, shop1.numIndices, &shop1.vertexBuffer, &shop1.indexBuffer, modelViewLocation, invModelViewLocation, shop1.modelView, shop1.invModelView, &camera);
shop1Mesh.render();
setVariables(boden.modelViewProj, projection, boden.model, modelViewProjLocation, boden.numIndices, &boden.vertexBuffer, &boden.indexBuffer, modelViewLocation, invModelViewLocation, boden.modelView, boden.invModelView, &camera);
bodenMesh.render();
*/