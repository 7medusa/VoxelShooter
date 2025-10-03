#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>

// Utility: ensure SDL_ttf is initialized (idempotent)
inline bool EnsureTTFInit(std::string* outError = nullptr) {
    if (TTF_WasInit()) return true;
    if (TTF_Init() == 0) return true;
    if (outError) *outError = TTF_GetError();
    return false;
}

// Create an OpenGL texture from a text string using SDL_ttf.
// Parameters:
// - text: UTF-8 string to render
// - fontPath: path to a .ttf/.otf font file
// - fontSize: size in points (typographic points)
// - color: SDL_Color for the text (RGBA)
// Outputs:
// - outTexture: generated GL texture id (GL_TEXTURE_2D). Caller owns it and must delete with glDeleteTextures(1, &tex).
// - outWidth/outHeight: pixel dimensions of the generated texture (optional; pass nullptr to ignore)
// Returns true on success, false on failure.
inline bool CreateTextTexture(const std::string& text,
                              const std::string& fontPath,
                              int fontSize,
                              SDL_Color color,
                              GLuint* outTexture,
                              int* outWidth = nullptr,
                              int* outHeight = nullptr,
                              std::string* outError = nullptr) {
    if (!outTexture) {
        if (outError) *outError = "outTexture is null";
        return false;
    }

    std::string err;
    if (!EnsureTTFInit(&err)) {
        if (outError) *outError = std::string("TTF_Init failed: ") + err;
        return false;
    }

    TTF_Font* font = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!font) {
        if (outError) *outError = std::string("TTF_OpenFont failed: ") + TTF_GetError();
        return false;
    }

    // Render text to an SDL_Surface (blended for best quality)
    SDL_Surface* surf = nullptr;
    if (!text.empty())
        surf = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    else {
        // Render a minimal 1x1 transparent surface for empty text
        surf = SDL_CreateRGBSurfaceWithFormat(0, 1, 1, 32, SDL_PIXELFORMAT_RGBA32);
        if (surf) SDL_FillRect(surf, nullptr, SDL_MapRGBA(surf->format, 0, 0, 0, 0));
    }

    if (!surf) {
        if (outError) *outError = std::string("TTF_RenderUTF8_Blended failed: ") + TTF_GetError();
        TTF_CloseFont(font);
        return false;
    }

    // Convert to a known pixel format (RGBA32) for OpenGL upload
    SDL_Surface* conv = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surf);
    if (!conv) {
        if (outError) *outError = std::string("SDL_ConvertSurfaceFormat failed: ") + SDL_GetError();
        TTF_CloseFont(font);
        return false;
    }

    const int w = conv->w;
    const int h = conv->h;
    if (outWidth) *outWidth = w;
    if (outHeight) *outHeight = h;

    // Generate GL texture
    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Align to byte boundaries to avoid row padding issues
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Upload pixels
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, conv->pixels);

    // Cleanup
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(conv);
    TTF_CloseFont(font);

    *outTexture = tex;
    return true;
}

// Convenience overload that keeps the font open if you need to render multiple strings with the same font.
// Caller owns the font and must call TTF_CloseFont when done.
inline TTF_Font* LoadFont(const std::string& fontPath, int fontSize, std::string* outError = nullptr) {
    std::string err;
    if (!EnsureTTFInit(&err)) {
        if (outError) *outError = std::string("TTF_Init failed: ") + err;
        return nullptr;
    }
    TTF_Font* f = TTF_OpenFont(fontPath.c_str(), fontSize);
    if (!f && outError) *outError = std::string("TTF_OpenFont failed: ") + TTF_GetError();
    return f;
}

inline bool CreateTextTextureWithFont(TTF_Font* font,
                                      const std::string& text,
                                      SDL_Color color,
                                      GLuint* outTexture,
                                      int* outWidth = nullptr,
                                      int* outHeight = nullptr,
                                      std::string* outError = nullptr) {
    if (!font) {
        if (outError) *outError = "font is null";
        return false;
    }
    if (!outTexture) {
        if (outError) *outError = "outTexture is null";
        return false;
    }

    SDL_Surface* surf = nullptr;
    if (!text.empty())
        surf = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    else {
        surf = SDL_CreateRGBSurfaceWithFormat(0, 1, 1, 32, SDL_PIXELFORMAT_RGBA32);
        if (surf) SDL_FillRect(surf, nullptr, SDL_MapRGBA(surf->format, 0, 0, 0, 0));
    }

    if (!surf) {
        if (outError) *outError = std::string("TTF_RenderUTF8_Blended failed: ") + TTF_GetError();
        return false;
    }

    SDL_Surface* conv = SDL_ConvertSurfaceFormat(surf, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(surf);
    if (!conv) {
        if (outError) *outError = std::string("SDL_ConvertSurfaceFormat failed: ") + SDL_GetError();
        return false;
    }

    const int w = conv->w;
    const int h = conv->h;
    if (outWidth) *outWidth = w;
    if (outHeight) *outHeight = h;

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, conv->pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    SDL_FreeSurface(conv);

    *outTexture = tex;
    return true;
}
