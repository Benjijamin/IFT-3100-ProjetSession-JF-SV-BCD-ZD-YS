#pragma once

#include "Ray.h"
#include "RayHit.h"
#include <array>

class Cube {
public:
    glm::vec3 center;
    float size;  // Half-width of the cube
    glm::mat4 transform;  // For rotated cubes

    Cube(const glm::vec3& c, float s) : center(c), size(s), transform(1.0f) {}

    Cube(const glm::vec3& c, float s, const glm::mat4& t) : center(c), size(s), transform(t) {}

    bool intersect(const Ray& ray, RayHit& hit) const {
        // Transform ray to object space if the cube is transformed
        Ray localRay = ray;
        glm::mat4 invTransform = glm::inverse(transform);

        if (transform != glm::mat4(1.0f)) {
            glm::vec3 localOrigin = glm::vec3(invTransform * glm::vec4(ray.getOrigin(), 1.0f));
            glm::vec3 localDir = glm::vec3(invTransform * glm::vec4(ray.getDirection(), 0.0f));
            localRay = Ray(localOrigin, localDir);
        }

        // Calculate box bounds
        glm::vec3 boxMin = center - glm::vec3(size);
        glm::vec3 boxMax = center + glm::vec3(size);

        glm::vec3 rayOrigin = localRay.getOrigin();
        glm::vec3 rayDir = localRay.getDirection();
        glm::vec3 invDir = 1.0f / rayDir;

        // Calculate t values for each slab
        glm::vec3 t1 = (boxMin - rayOrigin) * invDir;
        glm::vec3 t2 = (boxMax - rayOrigin) * invDir;

        // Find the largest tMin and smallest tMax
        glm::vec3 tMin = glm::min(t1, t2);
        glm::vec3 tMax = glm::max(t1, t2);

        float tNear = glm::max(glm::max(tMin.x, tMin.y), tMin.z);
        float tFar = glm::min(glm::min(tMax.x, tMax.y), tMax.z);

        // No intersection if tNear > tFar or tFar < 0
        if (tNear > tFar || tFar < 0) {
            return false;
        }

        // Check if this is closer than any previous hit
        float t = (tNear > 0) ? tNear : tFar;
        if (t < hit.distance) {
            hit.hit = true;
            hit.distance = t;
            hit.position = ray.getPointAtDistance(t);

            // Calculate normal based on which face was hit
            glm::vec3 localHitPos = localRay.getPointAtDistance(t);
            glm::vec3 localNormal;

            // Determine which face was hit by checking which component is at the boundary
            float epsilon = 0.0001f;
            if (std::abs(localHitPos.x - boxMin.x) < epsilon) {
                localNormal = glm::vec3(-1, 0, 0);
            }
            else if (std::abs(localHitPos.x - boxMax.x) < epsilon) {
                localNormal = glm::vec3(1, 0, 0);
            }
            else if (std::abs(localHitPos.y - boxMin.y) < epsilon) {
                localNormal = glm::vec3(0, -1, 0);
            }
            else if (std::abs(localHitPos.y - boxMax.y) < epsilon) {
                localNormal = glm::vec3(0, 1, 0);
            }
            else if (std::abs(localHitPos.z - boxMin.z) < epsilon) {
                localNormal = glm::vec3(0, 0, -1);
            }
            else {
                localNormal = glm::vec3(0, 0, 1);
            }

            // Transform normal back to world space
            if (transform != glm::mat4(1.0f)) {
                localNormal = glm::normalize(glm::vec3(glm::transpose(invTransform) * glm::vec4(localNormal, 0.0f)));
            }

            hit.normal = localNormal;
            return true;
        }
        return false;
    }
};