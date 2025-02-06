// DessinVectoriel.h
// Interface de la création de primitives 2D

#pragma once

#include "ofMain.h"
#include "Editor.h"

// Types de primitive 2D : point, ligne, rectangle, ellipse et triangle équilatéral
enum class TypePrimitive2D { none, point, ligne, rectangle, ellipse, triangle };

class DessinVectoriel : public Editor {
public:
    void setup() override;
    void update() override;
    void draw() override;
    void drawGui() override;
    void exit() override;

    void mouseDragged(int x, int y, int button) override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;

    void load(const std::string& path) override;
    void save(const std::string& path) override;

private:
    TypePrimitive2D mode;
    float epaisseur_trait;

    int clicX;
    int clicY;
    bool clic;

    int curseurX;
    int curseurY;
};
