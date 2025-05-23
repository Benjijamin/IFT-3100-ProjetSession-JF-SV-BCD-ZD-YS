#include "Ray.h"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin(origin), direction(glm::normalize(direction)) {}

glm::vec3 Ray::getOrigin() const {
    return origin;
}

glm::vec3 Ray::getDirection() const {
    return direction;
}

glm::vec3 Ray::getPointAtDistance(float t) const {
    return origin + direction * t;
}