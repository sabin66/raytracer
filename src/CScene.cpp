//
//  CScene.cpp
//  rt
//
//  Created by Radoslaw Mantiuk on 22/01/2023.
//

#include "rt.h"

#include<fstream>

/// \fn create(void)
/// \brief Adds components to the scene.
///
void CScene::create() {
    objectList.clear();
    lightList.clear();

    CSphere* sphere1 = new CSphere({-1, 0, 5}, 1.0f);
    sphere1->matAmbient = glm::vec3(0.0f, 0.1f, 0.0f);
    sphere1->matDiffuse = glm::vec3(0.0f, 0.6f, 0.0f);
    sphere1->matSpecular = glm::vec3(0.7f, 0.7f, 0.7f);
    sphere1->matShininess = 32.0f;
    sphere1->reflectance = 0.0f;
    objectList.push_back(sphere1);

    CSphere* sphere2 = new CSphere({0, 0, 3}, 1.0f);
    sphere2->matAmbient = glm::vec3(0.1f, 0.0f, 0.0f);
    sphere2->matDiffuse = glm::vec3(0.6f, 0.0f, 0.0f);
    sphere2->matSpecular = glm::vec3(0.7f, 0.7f, 0.7f);
    sphere2->matShininess = 30.0f;
    //sphere2->isTexture = true;
    //sphere2->texture = CImage::createTexture(400,400);
    objectList.push_back(sphere2);

    CSphere* sphere3 = new CSphere({2, 0, 1}, 1.0f);
    sphere3->matAmbient = glm::vec3(0.0f, 0.0f, 0.1f);
    sphere3->matDiffuse = glm::vec3(0.0f, 0.0f, 0.6f);
    sphere3->matSpecular = glm::vec3(0.7f, 0.7f, 0.7f);
    sphere3->matShininess = 30.0f;
    //sphere3->isTexture = true;
    //sphere3->texture = CImage::createTexture(400,400);
    objectList.push_back(sphere3);

    CTriangle* triangle1 = new CTriangle({5, 5, -5}, {-5, 5, -5}, {-5, -5, -5});
    triangle1->matAmbient = glm::vec3(0.1f, 0.0f, 0.0f);
    triangle1->matDiffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    triangle1->matSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
    triangle1->matShininess = 0.0f;
    triangle1->reflectance = 1.0f;
    objectList.push_back(triangle1);

    CTriangle* triangle2 = new CTriangle({5, 5, -5}, {-5, -5, -5}, {5, -5, -5});
    triangle2->matAmbient = glm::vec3(0.1f, 0.0f, 0.0f);
    triangle2->matDiffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    triangle2->matSpecular = glm::vec3(0.0f, 0.0f, 0.0f);
    triangle2->matShininess = 0.0f;
    triangle2->reflectance = 1.0f;
    objectList.push_back(triangle2);

    CLight light1(glm::vec3(-3, -2, 8));
    light1.color = glm::vec3(0.6f, 0.6f, 0.6f);
    lightList.push_back(light1);

    CLight light2(glm::vec3(5, -2, 8));
    light2.color = glm::vec3(0.3f, 0.3f, 0.3f);
    lightList.push_back(light2);
}


