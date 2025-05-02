#pragma once

#include "Ray.h"
#include "RayHit.h"

class Sphere {
public:
    glm::vec3 center;
    float radius;

    Sphere(const glm::vec3& c, float r) : center(c), radius(r) {}

    bool intersect(const Ray& ray, RayHit& hit) const {
        glm::vec3 oc = ray.getOrigin() - center;
        float a = glm::dot(ray.getDirection(), ray.getDirection());
        float b = 2.0f * glm::dot(oc, ray.getDirection());
        float c = glm::dot(oc, oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) return false;

        float sqrtD = sqrt(discriminant);
        float t1 = (-b - sqrtD) / (2.0f * a);
        float t2 = (-b + sqrtD) / (2.0f * a);
        float t = (t1 > 0) ? t1 : t2;
        if (t < 0) return false;

        if (t < hit.distance) {
            hit.hit = true;
            hit.distance = t;
            hit.position = ray.getPointAtDistance(t);
            hit.normal = glm::normalize(hit.position - center);
            return true;
        }
        return false;
    }
};
