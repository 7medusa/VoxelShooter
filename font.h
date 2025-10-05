#pragma once
#include "libs/stb_truetype.h"
#include <string>
#include <GL/glew.h>

class Shader;
struct SDL_Window;

class Font {
public:
    Font(const char* filename, float size);
    void drawText(float x, float y, const char* text, Shader* fontShader);
    void fontDraw(Shader* fontShader, SDL_Window* window, Font* font, std::string text, float x, float y);
    float measureTextWidth(const char* text, const stbtt_bakedchar* cdata);
    void loading(Shader* fontShader, SDL_Window* window, Font font, unsigned int windowWidth, unsigned int windowHeight, const char* text);
    stbtt_bakedchar cdata[96];

private:
    struct FontVertex;
    GLuint fontTexture;
    GLuint fontVao;
    GLuint fontVertexBufferId;
    FontVertex* fontVertexBufferData;
    int fontVertexBufferCapacity;
};