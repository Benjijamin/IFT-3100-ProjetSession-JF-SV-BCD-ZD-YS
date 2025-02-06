#include "FreeFlightCamera.h"

FreeFlightCamera::FreeFlightCamera()
    : moveSpeed(10.0f), sensitivity(0.1f) {}

void FreeFlightCamera::setup() {
    lastMouse = glm::vec2(ofGetMouseX(), ofGetMouseY());
}

void FreeFlightCamera::update() {
    handleKeyboardInput();
    if (mouseControlEnabled) {
        handleMouseInput();
    }
}

void FreeFlightCamera::enableMouseControl() {
    mouseControlEnabled = true;
    lastMouse = glm::vec2(ofGetMouseX(), ofGetMouseY());
}

void FreeFlightCamera::disableMouseControl() {
    mouseControlEnabled = false;
}

float FreeFlightCamera::getMouseSpeed() const {
    return moveSpeed;
}

void FreeFlightCamera::setMouseSpeed(float speed) {
    moveSpeed = speed;
}

float FreeFlightCamera::getSensitivity() const {
    return sensitivity;
}

void FreeFlightCamera::setSensitivity(float sensitivity) {
    this->sensitivity = sensitivity;
}

void FreeFlightCamera::handleKeyboardInput() {
    if (ofGetKeyPressed('w')) {
        dolly(-moveSpeed);
    }
    if (ofGetKeyPressed('s')) {
        dolly(moveSpeed);
    }
    if (ofGetKeyPressed('a')) {
        truck(-moveSpeed);
    }
    if (ofGetKeyPressed('d')) {
        truck(moveSpeed);
    }
    if (ofGetKeyPressed('q')) {
        boom(-moveSpeed);
    }
    if (ofGetKeyPressed('e')) {
        boom(moveSpeed);
    }
}

void FreeFlightCamera::handleMouseInput() {
    glm::vec2 currentMouse = glm::vec2(ofGetMouseX(), ofGetMouseY());
    glm::vec2 delta = currentMouse - lastMouse;
    lastMouse = currentMouse;

    if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
        float yaw = delta.x * sensitivity;
        float pitch = delta.y * sensitivity;

        glm::quat qYaw = glm::angleAxis(ofDegToRad(yaw), glm::vec3(0, 1, 0));
        glm::quat qPitch = glm::angleAxis(ofDegToRad(pitch), glm::vec3(1, 0, 0));

        glm::quat orientation = qYaw * getOrientationQuat() * qPitch;
        setOrientation(orientation);
    }
}
