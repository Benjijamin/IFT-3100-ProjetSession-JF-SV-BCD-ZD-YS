#pragma once

#include "ofMain.h"

class SceneNode;

//Structure pour stocker les informations d'intersection
struct RayHit {
    bool hit = false;
    float distance = FLT_MAX;
    glm::vec3 position;
    glm::vec3 normal;
    SceneNode* node = nullptr;
};