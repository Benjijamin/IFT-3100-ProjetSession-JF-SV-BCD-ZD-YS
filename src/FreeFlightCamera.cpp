#include "FreeFlightCamera.h"

FreeFlightCamera::FreeFlightCamera()
    : moveSpeed(10.0f), sensitivity(0.1f), mouseInputEnabled(false), eventsSet(false), events(nullptr) {}

void FreeFlightCamera::begin(const ofRectangle& viewport) {
    if (!eventsSet) {
        setEvents(ofEvents());
    }
    ofCamera::begin(viewport);
}

void FreeFlightCamera::begin() {
    begin(getViewport());
}

void FreeFlightCamera::enableMouseInput() {
    if (!mouseInputEnabled && events) {
        listeners.push(events->update.newListener(this, &FreeFlightCamera::update));
        listeners.push(events->mouseDragged.newListener(this, &FreeFlightCamera::mouseDragged));
        listeners.push(events->mousePressed.newListener(this, &FreeFlightCamera::mousePressed));
    }
    mouseInputEnabled = true;
}

void FreeFlightCamera::disableMouseInput() {
    if (mouseInputEnabled && events) {
        listeners.unsubscribeAll();
    }
    mouseInputEnabled = false;
}

bool FreeFlightCamera::getMouseInputEnabled() const {
    return mouseInputEnabled;
}

void FreeFlightCamera::setEvents(ofCoreEvents& events) {
    bool wasMouseInputEnabled = mouseInputEnabled;
    disableMouseInput();
    this->events = &events;
    if (wasMouseInputEnabled) {
        enableMouseInput();
    }
    eventsSet = true;
}

float FreeFlightCamera::getMoveSpeed() const {
    return moveSpeed;
}

void FreeFlightCamera::setMoveSpeed(float speed) {
    moveSpeed = speed;
}

float FreeFlightCamera::getSensitivity() const {
    return sensitivity;
}

void FreeFlightCamera::setSensitivity(float sensitivity) {
    this->sensitivity = sensitivity;
}

void FreeFlightCamera::update(ofEventArgs& args) {
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
    };
}

void FreeFlightCamera::mouseDragged(ofMouseEventArgs& args) {
    if (ofGetMousePressed(OF_MOUSE_BUTTON_LEFT)) {
        glm::vec2 currentMouse(args.x, args.y);
        glm::vec2 delta = currentMouse - lastMouse;
        lastMouse = currentMouse;

        float yaw = delta.x * sensitivity;
        float pitch = delta.y * sensitivity;

        glm::quat qYaw = glm::angleAxis(ofDegToRad(yaw), glm::vec3(0, 1, 0));
        glm::quat qPitch = glm::angleAxis(ofDegToRad(pitch), glm::vec3(1, 0, 0));

        glm::quat orientation = qYaw * getOrientationQuat() * qPitch;
        setOrientation(orientation);
    }
}

void FreeFlightCamera::mousePressed(ofMouseEventArgs& args) {
    lastMouse = glm::vec2(args.x, args.y);
}
