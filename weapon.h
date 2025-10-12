#pragma once
#include "string"

using namespace std;

class Weapon {
public:
    Weapon();
    void reload();
    int magazine;
    const int maxMagazine = 7;
    const string type = "pistol";
};