#include "BezierCurve.h"

void BezierCurve::setup()
{
    // Taille et espacement des fenêtres
    windowSize = ImVec2(400.0f, 175.0f);
    windowGap = 12.0f;
    windowPos = ImVec2(ofGetWidth() - windowSize.x - windowGap / 3, windowGap * 2);

    // Atrributs de la courbe
    lineRes = 100;
    lineColor = ofColor(255.0f);

    // Attributs des points de contrôle
    minPts = 3;
    maxPts = 9;
    nPts = 3;
    ptOffset = 100.0f;
    ptSize = 20.0f;
    ptColor = ofColor(255.0f, 0.0f, 0.0f);

    // Variables d'état
    active = false;
    editing = false;
}

void BezierCurve::draw()
{
    if (!active) return;

    // Tracer toutes les courbes stockées en mémoire
    for (const auto& c : curves)
    {
        ofFill();
        ofSetColor(c.color);
        c.line.draw();
    }

    // Tracer la courbe en cours de traçage
    if (editing)
    {
        drawCurrent();
    }
}

void BezierCurve::drawGui()
{
    if (!active || editing) return;

    // Drapeaux de la fenêtre
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoTitleBar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 12));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));

    // Paramètres de la fenêtre
    ImGui::SetNextWindowPos(windowPos);
    ImGui::SetNextWindowSize(windowSize);

    // Titre de la fenêtre
    ImGui::Begin("Bézier-style curve", nullptr, flags);
    
    // Boutons d'édition
    ImGui::SliderInt("Control points", &nPts, minPts, maxPts);
    ImGui::ColorEdit3("Line color", (float*) &lineColor);
    if (ImGui::Button("Add Bézier-style curve")) addCurve();

    // Fin
    ImGui::PopStyleVar(3);
    ImGui::End();
}

void BezierCurve::exit()
{
    active = false;
    curves.clear();
}

void BezierCurve::mouseDragged(int x, int y, int button)
{
    if (editing)
    {
        refreshCurve((float) x, (float) y);
    }
}

void BezierCurve::begin()
{
    active = true;
}

bool BezierCurve::isEditing() const
{
    return editing;
}

void BezierCurve::drawCurrent() const
{
    ofFill();
    ofSetColor(currentCurve.color);
    currentCurve.line.draw();

    ofSetColor(ptColor);
    for (const auto& pt : currentCurve.pts)
    {
        ofDrawEllipse(pt, ptSize, ptSize);
    }
}

void BezierCurve::addCurve()
{
    editing = true;
    Curve c;
    c.color = lineColor;

    for (int i = 0; i <= lineRes; ++i)
    {
        c.line.addVertex(ofPoint());
    }

    initPts(c);
    computeCurve(c);
    currentCurve = c;
}

void BezierCurve::initPts(Curve& c) const
{
    ofVec2f center = ofVec2f(ofGetWidth() / 2.0f, ofGetHeight() / 2.0f);
    int offset = 0;
    int delta = -1;

    for (int i = 0; i < nPts; ++i)
    {
        c.pts.push_back(ofVec2f(center.x + (float) offset * ptOffset, center.y));
        offset = ceil((float) i / 2.0f) * delta;
        delta *= -1;
    }
}

const float BezierCurve::fact(const int n) const
{
    if (n <= 1) return 1.0f;
    return (float) n * fact(n - 1);
}

const float BezierCurve::Bernstein(const int n, const int k, const float t) const
{
    return (fact(n) / (fact(k) * fact(n - k))) * pow(t, k) * pow(1 - t, n - k);
}

void BezierCurve::computeFragment(Curve& c, const int index) const
{
    float t = (float) index / lineRes;
    ofVec2f coords = ofVec2f(0.0f, 0.0f);
    size_t n = c.pts.size() - 1;

    for (size_t k = 0; k <= n; ++k)
    {
        coords += Bernstein(n, k, t) * c.pts[k];
    }

    c.line[index] = { coords.x, coords.y, 0.0f };
}

void BezierCurve::computeCurve(Curve& c) const
{
    for (int i = 0; i <= lineRes; ++i)
    {
        computeFragment(c, i);
        ofLog() << "Ligne, point " << i << " - x: " << c.line[i].x << ", y: " << c.line[i].y;
    }
}

void BezierCurve::refreshCurve(const float x, const float y)
{
    std::vector<ofVec2f> v;
    for (const auto& pt : currentCurve.pts)
    {
        bool clampX = x > pt.x - ptSize && x < pt.x + ptSize;
        bool clampY = y > pt.y - ptSize && y < pt.y + ptSize;
        clampX && clampY ? v.push_back(ofVec2f(x, y)) : v.push_back(pt);
    }
    currentCurve.pts = v;
    computeCurve(currentCurve);
}

void BezierCurve::applyCurve()
{
    editing = false;
    curves.push_back(currentCurve);
}

void BezierCurve::update() {}
void BezierCurve::mousePressed(int x, int y, int button) {}
void BezierCurve::mouseReleased(int x, int y, int button) {}
void BezierCurve::mouseScrolled(int x, int y, float scrollX, float scrollY) {}
void BezierCurve::load(const std::string& path) {}
void BezierCurve::save(const std::string& path) {}