#pragma once
#include "ofMain.h"

class FreeFlightCamera : public ofCamera {
public:
    FreeFlightCamera();

    virtual void begin(const ofRectangle& viewport);
    virtual void begin();

    void enableMouseInput();
    void disableMouseInput();
    bool getMouseInputEnabled() const;
    void setEvents(ofCoreEvents& events);

    float getMoveSpeed() const;
    void setMoveSpeed(float speed);

    float getSensitivity() const;
    void setSensitivity(float sensitivity);

private:
    void update(ofEventArgs& args);
    void mouseDragged(ofMouseEventArgs& args);
    void mousePressed(ofMouseEventArgs& args);

    float moveSpeed;
    float sensitivity;

    glm::vec2 lastMouse;

    bool mouseInputEnabled;
    bool eventsSet;
    ofCoreEvents* events;
    ofEventListeners listeners;
};
