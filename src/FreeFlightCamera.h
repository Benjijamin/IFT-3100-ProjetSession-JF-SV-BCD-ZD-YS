#pragma once

#include "ofMain.h"

class FreeFlightCamera : public ofCamera {
public:
    FreeFlightCamera();

    void setup();
    void update();
    void enableMouseControl();
    void disableMouseControl();

    float getMouseSpeed() const;
    void setMouseSpeed(float speed);

    float getSensitivity() const;
    void setSensitivity(float sensitivity);

private:
    void handleKeyboardInput();
    void handleMouseInput();

    glm::vec2 lastMouse;
    bool mouseControlEnabled = false;

    float moveSpeed;
    float sensitivity;
};
