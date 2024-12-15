#pragma once
#include "cgmath.h"

class Light {
public:
    Vec3 direction;  // Direction of the light 
    Vec3 color;     // Light color 
    float intensity; // Brightness multiplier

    // fixed directional light
    Light(Vec3 pos = Vec3(0.f, -1.0f, 0.f), Vec3 col = Vec3(0.8, 0.8, 0.8), float intens = 1.0f)
        : direction(pos), color(col), intensity(intens) {}
};