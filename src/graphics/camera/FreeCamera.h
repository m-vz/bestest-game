//
// Created by Milan van Zanten on 16.03.18.
//

#ifndef BESTEST_GAME_FREECAMERA_H
#define BESTEST_GAME_FREECAMERA_H

#include "Camera.h"
#include "../../io/Window.h"
#include "../../io/Mouse.h"
#include "../../io/Keyboard.h"

class FreeCamera : public Camera {
public:
    float speed = 30.0f, friction = 0.1f;

    FreeCamera(Window &capturedIn, Mouse &mouse, Keyboard &keyboard, glm::vec3 position, glm::vec2 angles);
    void changeAspect(int width, int height) override;
    void update(long long int time) override;

private:
    bool mouseCaptured = true;
    Window &capturedIn;
    /// Contains angle around the x axis and angle around the y axis.
    glm::vec2 angles;
    glm::vec3 acceleration = glm::vec3(0.0f), accelerationTmp;
    glm::vec3 position, velocity = glm::vec3(0.0f), lookingDirection, rightDirection, upDirection = glm::vec3(0.0f, 1.0f, 0.0f);
};

#endif //BESTEST_GAME_FREECAMERA_H