#pragma once
// Camera.h
#pragma once
#include "cgmath.h"
#include "Collision.h"

class Camera {
public:
    Vec3 position;
    Vec3 target;
    Vec3 up;
    float yaw, pitch;
    float moveSpeed, rotateSpeed;
    AABB box{ (position - Vec3(0.5f, 1.0f, 0.5f)), (position + Vec3(0.5f, 0.0f, 0.5f)) };

    Camera(Vec3 pos = Vec3(0, 2, 5), Vec3 tar = Vec3(0, 0, 0), Vec3 u = Vec3(0, 1, 0))
        : position(pos), target(tar), up(u), yaw(0), pitch(0), moveSpeed(5.0f), rotateSpeed(0.001f) {}

    
    // get the lookat matrix in math lib
    Matrix getLookat() {
        Matrix v;
        Matrix v1 = v.lookAt(position, target, up);
        return v1;
    }

    void move(Vec3 direction, float deltaTime) {
        position += direction * (moveSpeed * deltaTime);
        target += direction * (moveSpeed * deltaTime);
    }

    void rotate(float xOffset, float yOffset) {
        yaw += xOffset * rotateSpeed;
        pitch += yOffset * rotateSpeed;

        // limit the rotate angle
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        // copute the direction into Vec3 and compute target
        Vec3 direction;
        direction.x = cos(yaw) * cos(pitch);
        direction.y = sin(pitch);
        direction.z = sin(yaw) * cos(pitch);
        target = position + direction.normalize();
    }

    // get direction of camera
    Vec3 getForwardDirection() const {
        return Vec3(
            cos(yaw) * cos(pitch),
            sin(pitch),
            sin(yaw) * cos(pitch)
        ).normalize();
    }

    Vec3 getRightDirection() const {
        return Vec3(
            cos(yaw - M_PI_2), // Subtract 90 degrees (in radians)
            0,
            sin(yaw - M_PI_2)
        ).normalize();
    }

    // bounding box moves with the camera
    void updateBoundingBox() {
        box.updatePos((position - Vec3(0.5f, 1.0f, 0.5f)), (position + Vec3(0.5f, 0.0f, 0.5f)));
    }

};
