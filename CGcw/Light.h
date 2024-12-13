#pragma once
#include "cgmath.h"

class Light {
public:
    Vec3 position;  // Position of the light (world space)
    Vec3 color;     // Light color (RGB, e.g., white = (1.0, 1.0, 1.0))
    float intensity; // Brightness multiplier

    Light(Vec3 pos = Vec3(0.f, -1.0f, 0.f), Vec3 col = Vec3(1.5, 1.5, 1.5), float intens = 1.0f)
        : position(pos), color(col), intensity(intens) {}
};