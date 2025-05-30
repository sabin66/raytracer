//
//  CObject.cpp
//  rt
//
//  Created by Radoslaw Mantiuk on 22/01/2023.
//

#include "rt.h"
#include "CObject.hpp"


/// \fn intersect(CRay ray)
/// \brief Computes intersection between ray and sphere.
/// \param ray Ray parameters.
/// \return Distance from camera position to the closest intersection point, or negative value.
///
float CSphere::intersect(const CRay& ray) {
    float t = 1;
    glm::vec3 v = ray.pos - pos;
    float A = glm::dot(ray.dir, ray.dir);
    float B = 2.0f * glm::dot(v, ray.dir);
    float C = glm::dot(v, v) - r * r;

    float delta = B * B - 4 * A * C;
    if(delta < 0) return -t;
    float t1 = (-B - sqrt(delta)) / (2.0f * A);
    float t2 = (-B + sqrt(delta)) / (2.0f * A);
    if(t1 > t2) {
        return t2;
    }
    else {
        return t1;
    }

}


/// Normal vector to the sphere surface
/// \fn normal(glm::vec3 hit_pos)
/// \brief Surface normal vector at the intersection point.
/// \param hit_pos Intersection point.
/// \return Normal vector parameters.
///
glm::vec3 CSphere::normal(const glm::vec3& hit_pos) {
    glm::vec3 n = {0,0,0};
    n = glm::normalize(hit_pos - pos);
    return n;
}

/// Computes texture mapping coordinates (u,v).
/// \param normal_vec Normalized normal vector at intersection point.
/// \return (u,v) texture coordinates in <0,1> range.
glm::vec2 CSphere::textureMapping(const glm::vec3& normal_vec) {
    glm::vec2 uv = {0,0};
    uv.x = 0.5 + glm::asin(normal_vec.x)/3.1415926535897932384626433832795;
    uv.y = 0.5 -glm::asin(normal_vec.y)/3.1415926535897932384626433832795;

    return uv;
}



/// \fn intersect(CRay ray)
/// \brief Computes intersection between triangle and sphere.
/// \param ray Ray parameters.
/// \return Distance from camera position to the closest intersection point, or negative value.
///
float CTriangle::intersect(const CRay& ray) {
    float t = 1;
    glm::vec3 baryPosition;
    bool hit = glm::intersectRayTriangle(ray.pos,ray.dir,v0,v1,v2,baryPosition);
    if(!hit) return -t;
    return baryPosition.z;
}


/// \fn normal(glm::vec3 hit_pos)
/// \brief Surface normal vector at the intersection point.
/// \param hit_pos Intersection point (not used for triangle).
/// \return Normal vector parameters.
///
glm::vec3 CTriangle::normal(const glm::vec3& hit_pos) {
    glm::vec3 n;
    glm::vec3 u = v1-v0;
    glm::vec3 v = v2-v1;
    n = glm::normalize(glm::cross(u,v));
    return n;
}



