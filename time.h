#pragma once

class Time {
public:
    Time();
    void update();
    void end();
    float* returnDelta();

    int fps;
    float time;
    float delta;
    double lastCounter;
    double perfCounterFrequency;
    double endCounter;
    double counterElapsed;
};