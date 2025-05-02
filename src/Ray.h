#pragma once

#include "ofMain.h"

//Représente un rayon
class Ray {
public:
    Ray::Ray(const glm::vec3& origin, const glm::vec3& direction);

    glm::vec3 getOrigin() const;
    glm::vec3 getDirection() const;
    glm::vec3 getPointAtDistance(float t) const;

private:
    glm::vec3  origin;
    glm::vec3  direction;
};
