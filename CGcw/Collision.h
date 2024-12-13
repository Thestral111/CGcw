#pragma once
#include "cgmath.h"


class AABB {
public:
    Vec3 min; // Minimum corner
    Vec3 max; // Maximum corner


    bool checkAABBCollision(const AABB& box1, const AABB& box2) {
        // Check for overlap in the x-axis
        if (box1.max.x < box2.min.x || box1.min.x > box2.max.x) return false;

        // Check for overlap in the y-axis
        if (box1.max.y < box2.min.y || box1.min.y > box2.max.y) return false;

        // Check for overlap in the z-axis
        if (box1.max.z < box2.min.z || box1.min.z > box2.max.z) return false;

        // If overlap exists in all axes, the boxes collide
        return true;
    }

};

