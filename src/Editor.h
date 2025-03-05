#pragma once

#include <string>

class Editor {
public:
    virtual ~Editor() {}

    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
    virtual void drawGui() = 0;
    virtual void exit() = 0;

    virtual void mouseDragged(int x, int y, int button) = 0;
    virtual void mousePressed(int x, int y, int button) = 0;
    virtual void mouseReleased(int x, int y, int button) = 0;
    virtual void mouseScrolled(int x, int y, float scrollX, float scrollY) = 0;

    virtual void load(const std::string& path) = 0;
    virtual void save(const std::string& path) = 0;
};
