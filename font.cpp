#include "font.h"
#include <cstdio>
#include <GL/glew.h>
#include "shader.h"
#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "defines.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "libs/stb_truetype.h"

struct Font::FontVertex {
    glm::vec2 position;
    glm::vec2 texCoords;
};

Font::Font(const char* filename, float size) {
    uint8_t ttfBuffer[1<<20];
    uint8_t tmpBitmap[512*512];

    fread(ttfBuffer, 1, 1<<20, fopen(filename, "rb"));
    stbtt_BakeFontBitmap(ttfBuffer, 0, size, tmpBitmap, 512, 512, 32, 96, cdata);

    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, tmpBitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenVertexArrays(1, &fontVao);
    glBindVertexArray(fontVao);
    glGenBuffers(1, &fontVertexBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, fontVertexBufferId);
    fontVertexBufferCapacity = 20;
    fontVertexBufferData = new FontVertex[fontVertexBufferCapacity * 6];
    glBufferData(GL_ARRAY_BUFFER, sizeof(FontVertex) * 6 * fontVertexBufferCapacity, 0, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(FontVertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(FontVertex), (void*)offsetof(FontVertex, texCoords));
    glBindVertexArray(0);
}

void Font::drawText(float x, float y, const char* text, Shader* fontShader) {
    glBindVertexArray(fontVao);
    glBindBuffer(GL_ARRAY_BUFFER, fontVertexBufferId);
    uint32_t len = strlen(text);
    if(fontVertexBufferCapacity < len) {
        fontVertexBufferCapacity = len;
        glBufferData(GL_ARRAY_BUFFER, sizeof(FontVertex) * 6 * fontVertexBufferCapacity, 0, GL_DYNAMIC_DRAW);
        delete[] fontVertexBufferData;
        fontVertexBufferData = new FontVertex[fontVertexBufferCapacity * 6];
    }
    GLCALL(glActiveTexture(GL_TEXTURE0));
    GLCALL(glBindTexture(GL_TEXTURE_2D, fontTexture));
    GLCALL(glUniform1i(glGetUniformLocation(fontShader->getShaderId(), "u_texture"), 0));

    FontVertex* vData = fontVertexBufferData;
    uint32_t numVertices = 0;
    while(*text) {
        if(*text >= 32 && *text < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata, 512, 512, *text - 32, &x, &y, &q, 1);

            vData[0].position = glm::vec2(q.x0, q.y0);vData[0].texCoords = glm::vec2(q.s0, q.t0);
            vData[1].position = glm::vec2(q.x1, q.y0);vData[1].texCoords = glm::vec2(q.s1, q.t0);
            vData[2].position = glm::vec2(q.x1, q.y1);vData[2].texCoords = glm::vec2(q.s1, q.t1);
            vData[3].position = glm::vec2(q.x0, q.y1);vData[3].texCoords = glm::vec2(q.s0, q.t1);
            vData[4].position = glm::vec2(q.x0, q.y0);vData[4].texCoords = glm::vec2(q.s0, q.t0);
            vData[5].position = glm::vec2(q.x1, q.y1);vData[5].texCoords = glm::vec2(q.s1, q.t1);

            vData += 6;
            numVertices += 6;
        }
        ++text;
    }

    GLCALL(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(FontVertex) * numVertices, fontVertexBufferData));
    GLCALL(glDrawArrays(GL_TRIANGLES, 0, numVertices));
}

void Font::fontDraw(Shader* fontShader, SDL_Window* window, Font* font, std::string text, float x, float y) {
    fontShader->bind();
    int w;
    int h;
    SDL_GetWindowSize(window, &w, &h);
    glm::mat4 ortho = glm::ortho(0.0f, (float)w, (float)h, 0.0f);
    GLCALL(glUniformMatrix4fv(glGetUniformLocation(fontShader->getShaderId(), "u_matrix"), 1, GL_FALSE, &ortho[0][0]));
    GLCALL(glDisable(GL_CULL_FACE));
    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCALL(glDisable(GL_DEPTH_TEST));

    const char* convertetText = text.c_str();
    font->drawText(x, y, convertetText, fontShader);

    GLCALL(glEnable(GL_CULL_FACE));
    GLCALL(glEnable(GL_DEPTH_TEST));
    Shader::unbind();
}

float Font::measureTextWidth(const char* text, const stbtt_bakedchar* cdata) {
    float w = 0.0f;
    for (const char* p = text; *p; ++p) {
        if (*p >= 32 && *p < 128) {
            w += cdata[*p - 32].xadvance; // Pixel
        }
    }
    return w;
}

void Font::loading(Shader* fontShader, SDL_Window* window, Font font, unsigned int windowWidth, unsigned int windowHeight, const char* text) {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    fontDraw(fontShader, window, &font, text, windowWidth / 2 - measureTextWidth(text, font.cdata) / 2, windowHeight / 2 - windowHeight / 7);
    SDL_GL_SwapWindow(window);
}
