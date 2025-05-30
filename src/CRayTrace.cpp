//
//  CRayTrace.cpp
//  rt
//
//  Created by Radoslaw Mantiuk on 22/01/2023.
//

#include "rt.h"


/// \fn compPrimaryRayMatrix(CCamera cam, glm::mat3& m)
/// \brief Computation of the projection matrix.
/// \param cam Camera parameters.
/// \param m Output projection matrix.
///
bool CRayTrace::compPrimaryRayMatrix(const CCamera& cam, glm::mat3& m) {
    glm::vec3 look = cam.lookp-cam.eyep;
    glm::vec3 u = (glm::cross(cam.up, look))/glm::length(glm::cross(cam.up, look));
    glm::vec3 v = (glm::cross(u, look)) / glm::length(glm::cross(u, look));
    glm::vec3 o = (look/glm::length(look) * (float(cam.width)/(2.0f*glm::tan((cam.fov*3.14f/180.0f)/2.0f)))) - ((float(cam.width)/2.0f*u)+(float(cam.height)/2.0f*v));
    m = (glm::mat3(u, v, o));
    // PRINT_VEC3("look",look);
    // PRINT_VEC3("u",u);
    // PRINT_VEC3("v",v);
    // PRINT_VEC3("o",o);
    return true;
}


/// \fn rayTrace(CScene scene, CRay& ray, COutput& out)
/// \brief Traces single ray.
/// \param scene Object with all scene components including a camera.
/// \param ray Ray parameter (primary and secondary rays).
/// \param out Object with output color and parameters used in recursion.
///
bool CRayTrace::rayTrace(const CScene& scene, CRay& ray, COutput& res) {
    float tmin =                    std::numeric_limits<float>::max();
    const int MAX_RAY_TREE =        5;
    const float MIN_RAY_ENERGY =      0.01f;
    const float EPS =               0.0001f;
    bool intersected =              false;
    CObject* hitObj =               nullptr;

    for(CObject *obj : scene.objectList) {
        float t = obj->intersect(ray);
        if(t > EPS && t < tmin) {
            tmin = t;
            intersected = true;
            hitObj = obj;
        }
    }

    if (intersected) {
        glm::vec3 p = ray.pos + tmin * ray.dir;
        glm::vec3 n = hitObj->normal(p);
        glm::vec3 V = -glm::normalize(ray.dir);
        glm::vec3 col(0.0f);
        // hitObj->matAmbient = glm::vec3(0.1f, 0.0f, 0.0f);
        // hitObj->matDiffuse = glm::vec3(0.4f, 0.4f, 0.4f);
        // hitObj->matSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
        // hitObj->matShininess = 0.0f;
        // hitObj->reflectance = 1.0f;

        for (const CLight& light : scene.lightList) {
            col += res.energy * hitObj->matAmbient * light.color;

            glm::vec3 L = glm::normalize(light.pos - p);

            CRay shadowRay;
            shadowRay.pos = p;
            shadowRay.dir = L;

            bool inShadow = false;
            for(CObject *obj : scene.objectList) {
                if(obj == hitObj) continue;

                float t = obj->intersect(shadowRay);
                if(t > EPS && t < tmin) {
                    inShadow = true;
                    break;
                }
            }
            if(!inShadow) {
                float cos_angle = glm::max(0.0f, glm::dot(n, L));
                col += res.energy * hitObj->matDiffuse * light.color * cos_angle;

                glm::vec3 h = glm::normalize(L + V);
                float cos_beta = glm::pow(glm::max(0.0f, glm::dot(n, h)), hitObj->matShininess);
                col += hitObj->matSpecular * light.color * cos_beta;
            }
        }
        if(hitObj->isTexture) {
            glm::vec2 uv = hitObj->textureMapping(n);
            glm::vec3 texCol = CImage::getTexel(hitObj->texture, uv.x, uv.y);
            col = col * texCol;
        }
        if(hitObj->reflectance > 0 && res.tree < MAX_RAY_TREE && res.energy > MIN_RAY_ENERGY) {
            res.tree++;
            res.energy *= hitObj->reflectance;
            CRay secondaryRay = reflectedRay(ray, n, p + n * EPS);
            rayTrace(scene, secondaryRay, res);
        }
        res.col = col + res.energy * res.col;
    } else {
        res.col = glm::vec3(0.0f);
    }

    return intersected;
}


/// \fn reflectedRay(CRay ray, glm::vec3 n, glm::vec3 pos)
/// \brief Computes parameters of the ray reflected at the surface point with given normal vector.
/// \param ray Input ray.
/// \param n Surface normal vector.
/// \param pos Position of reflection point.
/// \return Reflected ray.
///
CRay CRayTrace::reflectedRay(const CRay& ray, const glm::vec3& n, const glm::vec3& pos) {
    CRay reflected_ray;
    reflected_ray.pos = pos;
    glm::vec3 v = ray.dir;
    glm::vec3 r = v - 2 * glm::dot(v,n) * n;
    reflected_ray.dir = glm::normalize(r);
    return reflected_ray;
}

