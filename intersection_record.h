#ifndef INTERSECTION_RECORD_H_
#define INTERSECTION_RECORD_H_

#include <glm/glm.hpp>

#include "material.h"

struct IntersectionRecord
{
    double t_;
    glm::dvec3 position_;
    glm::dvec3 normal_;
    long unsigned int material_id_;
};

#endif /* INTERSECTION_RECORD_H_ */

