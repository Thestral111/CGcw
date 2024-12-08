#pragma once
// Camera.h
#pragma once
#include "cgmath.h"

class Camera {
public:
    Vec3 position;
    Vec3 target;
    Vec3 up;
    float yaw, pitch;
    float moveSpeed, rotateSpeed;

    Camera(Vec3 pos = Vec3(0, 2, 5), Vec3 tar = Vec3(0, 0, 0), Vec3 u = Vec3(0, 1, 0))
        : position(pos), target(tar), up(u), yaw(0), pitch(0), moveSpeed(5.0f), rotateSpeed(0.001f) {}

    Matrix getViewMatrix() {
        Vec3 direction = target - position;
        direction = direction.normalize();
        Vec3 right = up.Cross(direction).normalize();
        Vec3 adjustedUp = direction.Cross(right);
        Matrix view;
        view.m[0] = right.x; view.m[4] = right.y; view.m[8] = right.z; view.m[12] = -position.Dot(right);
        view.m[1] = adjustedUp.x; view.m[5] = adjustedUp.y; view.m[9] = adjustedUp.z; view.m[13] = -position.Dot(adjustedUp);
        view.m[2] = direction.x; view.m[6] = direction.y; view.m[10] = direction.z; view.m[14] = -position.Dot(direction);
        view.m[3] = 0; view.m[7] = 0; view.m[11] = 0; view.m[15] = 1;
        return view;
    }

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

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        Vec3 direction;
        direction.x = cos(yaw) * cos(pitch);
        direction.y = sin(pitch);
        direction.z = sin(yaw) * cos(pitch);
        target = position + direction.normalize();
    }

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

};
