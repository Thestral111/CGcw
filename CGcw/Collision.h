#pragma once
#include "cgmath.h"
#include <vector>


class AABB {
public:
    Vec3 min; // Minimum corner
    Vec3 max; // Maximum corner

    AABB(){
        min = Vec3(0, 0, 0);
        max = Vec3(0, 0, 0);
    }
    AABB(Vec3 _min, Vec3 _max) {
        min = _min;
        max = _max;
    }

    bool checkAABBCollision(const AABB& box1) {
        // Check for overlap in the each axis
        if (box1.max.x < min.x || box1.min.x > max.x) return false;

        if (box1.max.y < min.y || box1.min.y > max.y) return false;

        if (box1.max.z < min.z || box1.min.z > max.z) return false;

        // If overlap exists in all axes, the boxes collide
        return true;
    }

    AABB computeAABB(const std::vector<Vec3>& vertices) {
        Vec3 min = vertices[0];
        Vec3 max = vertices[0];

        // find the min and max value among all vertices
        for (const Vec3& v : vertices) {
            min.x = min(min.x, v.x);
            min.y = min(min.y, v.y);
            min.z = min(min.z, v.z);
            max.x = max(max.x, v.x);
            max.y = max(max.y, v.y);
            max.z = max(max.z, v.z);
        }

        return { min, max };
    }

    // transform the box by a matrix
    AABB transformAABB(Matrix& transform) {
        // the 8 corners of the box
        std::vector<Vec3> corners = {
            min,
            Vec3(max.x, min.y, min.z),
            Vec3(min.x, max.y, min.z),
            Vec3(min.x, min.y, max.z),
            Vec3(max.x, max.y, min.z),
            Vec3(max.x, min.y, max.z),
            Vec3(min.x, max.y, max.z),
            max
        };

        AABB transformedBox = { transform.mulVec(corners[0]), transform.mulVec(corners[7]) };
        // transform each corner
        for (const auto& corner : corners) {
            Vec3 transformedCorner = transform.mulVec(corner);
            transformedBox.min.x = min(transformedBox.min.x, transformedCorner.x);
            transformedBox.min.y = min(transformedBox.min.y, transformedCorner.y);
            transformedBox.min.z = min(transformedBox.min.z, transformedCorner.z);
            transformedBox.max.x = max(transformedBox.max.x, transformedCorner.x);
            transformedBox.max.y = max(transformedBox.max.y, transformedCorner.y);
            transformedBox.max.z = max(transformedBox.max.z, transformedCorner.z);
        }

        return transformedBox;
    }

    // update the box position
    void updatePos(Vec3 _min, Vec3 _max) {
        min = _min;
        max = _max;
    }

    // update position by a translation Vec3
    void updatePos(Vec3 trans) {
        min += trans;
        max += trans;
    }

};

