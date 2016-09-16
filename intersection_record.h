/*
 * hitrecord.h
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#ifndef INTERSECTION_RECORD_H_
#define INTERSECTION_RECORD_H_

#include <glm/glm.hpp>

#include "material2.h"

struct IntersectionRecord
{
    float t_;
    glm::vec3 position_;
    glm::vec3 normal_;
    Material material_;
};

#endif /* INTERSECTION_RECORD_H_ */

