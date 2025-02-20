#pragma once

#include "ofxImGui.h"

class ColorPicker {
public:
    void setup();
    void draw();
    void reset();

    ofFloatColor getColor() const;

private:
    ofFloatColor color;
    int currentItem;
};
