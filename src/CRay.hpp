//
//  CRay.hpp
//  rt
//
//  Created by Radoslaw Mantiuk on 22/01/2023.
//

#ifndef CRay_hpp
#define CRay_hpp

/// \class CRay
/// \brief Class with ray parameters.
///
class CRay{
public:
    
    glm::vec3 pos; ///< Position of vector origin.
    glm::vec3 dir; ///< Vector direction.
    CRay() : pos(0,0,0), dir(0,0,0) {}
    CRay(glm::vec3 pos, glm::vec3 dir) {
        this->pos = pos;
        this->dir = dir;
    }

    
};


#endif /* CRay_hpp */
