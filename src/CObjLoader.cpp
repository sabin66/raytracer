//
// Created by sd55617 on 24.04.25.
//
#include "rt.h"
#include "CObjLoader.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

bool CObjLoader::LoadOBJ(const std::string& filename, CScene& scene) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    } else {
        std::cout << "File opened successfully: " << filename << std::endl;
    }

    if (file.peek() == std::ifstream::traits_type::eof()) {
        std::cerr << "Error: File is empty" << std::endl;
        return false;
    }

    std::vector<glm::vec3> vertices;
    std::string line;
    int lineCount = 0;

    while (std::getline(file, line)) {
        lineCount++;

        std::cout << "  Processing: '" << line << "'" << std::endl;     // much needed comments for debugging because its tricky as hell

        std::istringstream ss(line);
        std::string token;
        ss >> token;

        std::cout << "  Token: '" << token << "'" << std::endl;

        if (token == "v") {
            double x, y, z;
            if (ss >> x >> y >> z) {
                glm::vec3 v(x, y, z);
                vertices.push_back(v);
                std::cout << "  Vertex added: " << v.x << ", " << v.y << ", " << v.z << std::endl;
            } else {
                std::cout << "  Failed to parse vertex coordinates" << std::endl;
            }
        }
    }

    std::cout << "Total lines read: " << lineCount << std::endl;
    std::cout << "Total vertices read: " << vertices.size() << std::endl;

    for (size_t i = 0; i + 2 < vertices.size(); i += 3) {
        std::vector<glm::vec3> tri = { vertices[i], vertices[i+1], vertices[i+2] };
        CTriangle* triangle = new CTriangle(tri);
        scene.objectList.push_back(triangle);
    }

    std::cout << "Total triangles created: " << scene.objectList.size() << "\n";
    return true;
}

