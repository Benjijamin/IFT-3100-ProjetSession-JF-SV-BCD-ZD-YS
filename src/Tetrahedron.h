#pragma once

#include "Ray.h"
#include "RayHit.h"
#include <array>

class Tetrahedron {
public:
    std::array<glm::vec3, 4> vertices;

    Tetrahedron(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3) {
        vertices[0] = v0;
        vertices[1] = v1;
        vertices[2] = v2;
        vertices[3] = v3;
    }

    // Simplified constructor that uses your existing tetrahedron primitive's vertices
    Tetrahedron(float size, const glm::vec3& center = glm::vec3(0.0f)) {
        vertices[0] = center + glm::vec3(size, size, size);
        vertices[1] = center + glm::vec3(size, -size, -size);
        vertices[2] = center + glm::vec3(-size, size, -size);
        vertices[3] = center + glm::vec3(-size, -size, size);
    }

    bool intersect(const Ray& ray, RayHit& hit) const {
        bool hasHit = false;

        // Test intersection with each of the 4 triangular faces
        // Face 0: vertices 0,1,2
        if (intersectTriangle(ray, vertices[0], vertices[1], vertices[2], hit)) {
            hasHit = true;
        }

        // Face 1: vertices 0,1,3
        if (intersectTriangle(ray, vertices[0], vertices[1], vertices[3], hit)) {
            hasHit = true;
        }

        // Face 2: vertices 0,2,3
        if (intersectTriangle(ray, vertices[0], vertices[2], vertices[3], hit)) {
            hasHit = true;
        }

        // Face 3: vertices 1,2,3
        if (intersectTriangle(ray, vertices[1], vertices[2], vertices[3], hit)) {
            hasHit = true;
        }

        return hasHit;
    }

private:
    // Möller–Trumbore algorithm for ray-triangle intersection
    bool intersectTriangle(const Ray& ray, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, RayHit& hit) const {
        const float EPSILON = 0.0000001f;
        glm::vec3 rayOrigin = ray.getOrigin();
        glm::vec3 rayDir = ray.getDirection();

        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;

        glm::vec3 h = glm::cross(rayDir, edge2);
        float a = glm::dot(edge1, h);

        // Ray parallel to triangle
        if (a > -EPSILON && a < EPSILON) {
            return false;
        }

        float f = 1.0f / a;
        glm::vec3 s = rayOrigin - v0;
        float u = f * glm::dot(s, h);

        // Intersection point outside triangle
        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        glm::vec3 q = glm::cross(s, edge1);
        float v = f * glm::dot(rayDir, q);

        // Intersection point outside triangle
        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        // Calculate t, ray intersection point
        float t = f * glm::dot(edge2, q);

        // Intersection point behind ray origin
        if (t <= EPSILON) {
            return false;
        }

        // Check if this is the closest intersection so far
        if (t < hit.distance) {
            hit.hit = true;
            hit.distance = t;
            hit.position = ray.getPointAtDistance(t);

            // Calculate surface normal
            hit.normal = glm::normalize(glm::cross(edge1, edge2));

            // Make sure the normal faces the ray
            if (glm::dot(hit.normal, rayDir) > 0) {
                hit.normal = -hit.normal;
            }

            return true;
        }

        return false;
    }
};