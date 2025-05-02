#pragma once

#include "ofMain.h"
#include "Ray.h"
#include "RayHit.h"
#include "SceneGraph.h"
#include "CameraManager.h"
#include "Sphere.h"


class RayTracer {
public:
    RayTracer();

    void setup(const SceneGraph& sceneGraph);
    void setCamera(ofCamera* camera);
    void setMaxDepth(int depth);
    void setImageSize(int width, int height);
    void setSceneGraph(SceneGraph* sceneGraph);

    void render();
    ofImage& getImage();
    Ray generateRay(int x, int y);
    RayHit intersectScene(const Ray& ray, const SceneGraph& sceneGraph);



private:
    glm::vec3 traceRay(const Ray& ray, int depth);

    glm::vec3 calculateLighting(const RayHit& hit, const Ray& ray, int depth);


    const SceneGraph* sceneGraph;
    ofCamera* camera;
    ofImage renderImage;

    int maxDepth;
    int width;
    int height;
};