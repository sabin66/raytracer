
#include <iostream>

#include <string>

#include <cmath>

#include <random>

#include "CObjLoader.hpp"

#include "rt.h"

extern void draw_robot();
extern void draw_triangles();
void generateImage(CScene &scene);  // using a simple double for-loop to generate an image

#ifdef _WIN32                                                                               // easiest way to dynamically change file paths
const std::string FILE_PATH = R"(I:\GIW2025_214A_Sobieranski_Dorian\raytracer\test.obj)";
#else
const std::string FILE_PATH = "../test.obj";
#endif

int main (int argc, char * const argv[]) {

    CScene scene;
    scene.create();

    // if (!CObjLoader::LoadOBJ(FILE_PATH, scene)) {
    //     std::cerr << "Failed to load OBJ file!" << std::endl;
    //     return 1;
    // }

    generateImage(scene);

    return 0;

}
void generateImage(CScene &scene) {
    COutput results;
    CRayTrace rt;
    CCamera cam(1000, 800, 50.0f, {0, -4, 15}, {0, 0, 0}, {0, 1, 0});
    glm::mat3 M;
    CRayTrace::compPrimaryRayMatrix(cam, M);
    CImage image(cam.width, cam.height);

    glm::vec3 w = glm::normalize(cam.eyep - cam.lookp); // direction
    glm::vec3 u = glm::normalize(glm::cross(cam.up, w)); // right
    glm::vec3 v = glm::cross(w,u); // up

    const int samplesPerPixel = 36;
    int pixelProgress = 0;

    cam.aperture = 0.5f;
    cam.focusDistance = 12.0f;

    for (int i = 0; i < cam.width; ++i) {
        for (int j = 0; j < cam.height; ++j) {
            glm::vec3 color = glm::vec3(0, 0, 0);
            for(int s = 0; s < samplesPerPixel; ++s) {

                float fx = (float)i + 0.5f;
                float fy = (float)j + 0.5f;

                glm::vec3 p(fx, fy, 1.0f);
                glm::vec3 primaryRay = glm::normalize(M*p);
                CRay ray;
                glm::vec3 aperturePoint = cam.eyep;
                glm::vec3 focalPoint = cam.eyep + primaryRay * cam.focusDistance;

                if(cam.aperture > 0.0f) {
                    
                    // generate random angle
                    float tetha = 2.0f * M_PI * rand()/float(RAND_MAX);
                    // generate random radius
                    float r = sqrt(rand()/float(RAND_MAX));
                    // assign aperturePoint to those values;
                    aperturePoint += (u * (r*cos(tetha)) + v * (r*sin(tetha))) * cam.aperture;
                    //aperturePoint += u * ((rand()/(float)RAND_MAX)* (rand()/(float)RAND_MAX)) +  v * ((rand()/(float)RAND_MAX)* (rand()/(float)RAND_MAX)) * cam.aperture;
                }

                ray.pos = aperturePoint;
                ray.dir = glm::normalize(focalPoint - aperturePoint);
                results.col = {0,0,0};
                results.energy = 1.0f;
                results.tree = 0;
                rt.rayTrace(scene,ray,results);

                color += results.col;
            }
            color = color/(float)samplesPerPixel;
            image.setPixel(i, j, color);
            pixelProgress++;
        }
        int progress = (pixelProgress*100)/(cam.width*cam.height);
        std::cout << "Progress : " << progress << "%" << std::endl;
    }
    std::cout << "render complete, displaying image..." << std::endl;
    cv::imshow("testing image generation", image.getImage());
    cv::waitKey();
}
void draw_triangles() {

    CImage img(1000, 1000);

    glm::vec3 color1(0.9,0.1,0.1); // red
    glm::vec3 color2(0.1,0.9,0.1); // green

    // draw circle
    glm::vec3 pp(0.0,0.0,1.0);
    float radius = 0.1f;
    img.drawCircle(pp, radius, color1);

    // triangle vertices
    glm::vec3 pp0(0.4, 0.3, 1);
    glm::vec3 pp1(-0.4, 0.3, 1);
    glm::vec3 pp2(0.4, -0.3, 1);

    // draws triangle in 2D
    img.drawLine(pp0, pp1, color1);
    img.drawLine(pp1, pp2, color1);
    img.drawLine(pp2, pp0, color1);

    // translation
    float tX = 0.2f; // OX translation
    float tY = 0.1f; // OY translation
    glm::mat3x3 mTrans {{1,0,0}, {0,1,0}, {tX,tY,1}}; // translation matrix
    //PRINT_MAT3(mTrans);

    // translation of vertices
    pp0 = mTrans * pp0;
    pp1 = mTrans * pp1;
    pp2 = mTrans * pp2;

    // draws triangle after translation
    img.drawLine(pp0, pp1, color2);
    img.drawLine(pp1, pp2, color2);
    img.drawLine(pp2, pp0, color2);

    img.save("robot.png");
    cv::imshow("ROBOT", img.getImage());
    cv::waitKey();

}
void draw_robot(){
    CImage img(1000, 1000);
    glm::vec3 color1(0.9,0.1,0.1);
    // Drawing rectangles ( hands )
    glm::vec3 p1(0.8,0.45,1.0);
    glm::vec3 p2(0.8,0.25,1.0);
    glm::vec3 p3(0.2,0.45,1.0);
    glm::vec3 p4(0.2,0.25,1.0);
    img.drawLine(p1, p3, color1);
    img.drawLine(p2, p4, color1);
    img.drawLine(p3, p4, color1);
    img.drawLine(p1, p2, color1);

    float tX = -1.0f; // OX translation
    float tY = 0.0f; // OY translation
    glm::mat3x3 mTrans {{1,0,0}, {0,1,0}, {tX,tY,1}}; // translation matrix
    PRINT_MAT3(mTrans);

    // translation of vertices
    p1 = mTrans * p1;
    p2 = mTrans * p2;
    p3 = mTrans * p3;
    p4 = mTrans * p4;
    img.drawLine(p1, p3, color1);
    img.drawLine(p2, p4, color1);
    img.drawLine(p3, p4, color1);
    img.drawLine(p1, p2, color1);

    ////////////////////////////////////////////////////////

    // Drawing head
    glm::vec3 pp(0.0,0.75,1.0);
    float radius = 0.2f;
    img.drawCircle(pp, radius, color1);

    // Drawing body
    glm::vec3 bodyPoint1(0.2,0.55,1.0);
    glm::vec3 bodyPoint2(-0.2,0.55,1.0);
    glm::vec3 bodyPoint3(0.2,-0.05,1.0);
    glm::vec3 bodyPoint4(-0.2,-0.05,1.0);

    img.drawLine(bodyPoint1,bodyPoint2,color1);
    img.drawLine(bodyPoint2,bodyPoint4,color1);
    img.drawLine(bodyPoint3,bodyPoint4,color1);
    img.drawLine(bodyPoint3,bodyPoint1,color1);

    /////////////////////////////////////////////////////

    // Drawing legs

    glm::vec3 legPoint1(0.2,-0.05,1.0);
    glm::vec3 legPoint2(0.05,-0.05,1.0);
    glm::vec3 legPoint3(0.3,-0.55,1.0);
    glm::vec3 legPoint4(0.15,-0.55,1.0);

    img.drawLine(legPoint1,legPoint3,color1);
    img.drawLine(legPoint3,legPoint4,color1);
    img.drawLine(legPoint4,legPoint2,color1);
    img.drawLine(legPoint2,legPoint1,color1);

    float tX2 = -0.0005f; // OX translation
    float tY2 = 0.0f; // OY translation


    glm::mat3x3 mMirror {{-1 ,0 ,0}, {0,1,0}, {0,0,1}};
    legPoint1 = mMirror * legPoint1;
    legPoint2 = mMirror * legPoint2;
    legPoint3 = mMirror * legPoint3;
    legPoint4 = mMirror * legPoint4;

    glm::mat3x3 mTrans2 {{1,0,0}, {0,1,0}, {tX2,tY2,1}}; // translation matrix
    legPoint1 = mTrans2 * legPoint1;
    legPoint2 = mTrans2 * legPoint2;
    legPoint3 = mTrans2 * legPoint3;
    legPoint4 = mTrans2 * legPoint4;


    img.drawLine(legPoint1,legPoint3,color1);
    img.drawLine(legPoint3,legPoint4,color1);
    img.drawLine(legPoint4,legPoint2,color1);
    img.drawLine(legPoint2,legPoint1,color1);


    img.save("robot.png");
    cv::imshow("ROBOT : ", img.getImage());
    cv::waitKey();

}
