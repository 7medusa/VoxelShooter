#include "time.h"
#include <SDL_timer.h>

Time::Time() {
    perfCounterFrequency = static_cast<double>(SDL_GetPerformanceFrequency());
    lastCounter = static_cast<double>(SDL_GetPerformanceCounter());
    delta = 0.0f;
    time = 0.0f;
    fps = 0.0f;
    endCounter = static_cast<double>(SDL_GetPerformanceCounter());
    counterElapsed = endCounter - lastCounter;
}

void Time::update() {
    time += delta;
}

void Time::end() {
    endCounter = static_cast<double>(SDL_GetPerformanceCounter());
    counterElapsed = endCounter - lastCounter;
    delta = static_cast<float>(counterElapsed) / static_cast<float>(perfCounterFrequency);
    lastCounter = endCounter;
    fps = 1.0f / delta;
}

float* Time::returnDelta() {
    return &delta;
}

float* Time::returnTime() {
    return &time;
}