#pragma once
#include "string"

using namespace std;

class Weapon {
public:
    Weapon();
    void reload();
    int magazine;
    const int maxMagazine = 7;
    const float reloadTime = 1.25f;
    const float shootTime = 0.8f;
    const string type = "pistol";
};