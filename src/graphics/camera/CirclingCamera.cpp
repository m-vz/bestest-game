//
// Created by Milan van Zanten on 14.03.18.
//

#include <thread>
#include "CirclingCamera.h"

CirclingCamera::CirclingCamera(int width, int height,
                               float distance, float angle, float speed,
                               glm::vec3 lookingAt)
        : distance(distance), angle(angle), speed(speed), lookingAt(lookingAt) {
    changeAspect(width, height);
    update(0);
}

void CirclingCamera::changeAspect(int width, int height) {
    // projection matrix: 45° Field of View, display range : 0.1 unit <-> 100 units
    projection = glm::perspective(glm::radians(45.0f), (float) width / (float) height, 0.1f, 10000.0f);
}

void CirclingCamera::update(long long int time) {
    float planeDistance = distance * std::cosf(angle), t = rotation + speed * speed * time/10000000000.0f;
    position = lookingAt - glm::vec3(planeDistance * sinf(t), distance*std::sinf(-angle), planeDistance * cosf(t));

    view = glm::lookAt(
            position, // camera position in world space
            lookingAt, // where the camera looks
            glm::vec3(0, 1, 0)  // head is up
    );
}

const glm::vec3 &CirclingCamera::getPosition() const {
    return position;
}

const glm::vec3 &CirclingCamera::getLookingAt() const {
    return lookingAt;
}
