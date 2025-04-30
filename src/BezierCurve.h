#pragma once

#include "ofMain.h"
#include "ofxImGui.h"
#include "Editor.h"

struct Curve {
    vector<ofVec2f> pts;
    ofColor color;
    ofPolyline line;
};

class BezierCurve : public Editor {
public:
    void setup() override;
    void draw() override;
    void drawGui() override;
    void exit() override;
    void mouseDragged(int x, int y, int button) override;

    void begin();
    void drawEditorWindow();
    const bool checkHover(const ImVec2& v) const;
    const bool isEditing() const;
    void drawCurrent() const;
    void toggleEditor();
    void initPts(Curve& c) const;
    const float fact(const int n) const;
    const float Bernstein(const int n, const int k, const float t) const;
    void computeFragment(Curve& c, const int index) const;
    void computeCurve(Curve& c) const;
    void refreshCurve(const float x, const float y);
    void quitEditor();

    // Méthodes inutilisées
    void update() override;
    void mousePressed(int x, int y, int button) override;
    void mouseReleased(int x, int y, int button) override;
    void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
    void load(const std::string& path) override;
    void save(const std::string& path) override;

private:

    // Taille et emplacement de la fenêtre
    float windowGap;
    ImVec2 windowPos;
    ImVec2 windowSize;

    // Attributs de la courbe
    int lineRes;
    // ofColor lineColor;
    Curve currentCurve;
    vector<Curve> curves;
    
    // Attributs des points de contrôle
    int minPts;
    int maxPts;
    int nPts;
    int ptOffset;
    float ptSize;
    ofColor ptColor;
    ofColor textColor;
    ofTrueTypeFont ptFont;

    // Variables d'état
    bool active;
    bool editing;
};