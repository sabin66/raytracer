//
// Created by sd55617 on 24.04.25.
//
#include "rt.h"
#ifndef COBJLOADER_HPP
#define COBJLOADER_HPP


#include <string>
#include <vector>
#include "CObject.hpp"

class CObjLoader {
public:
    static bool LoadOBJ(const std::string& filename,CScene& scene);
};

#endif //CObjLoader_HPP
