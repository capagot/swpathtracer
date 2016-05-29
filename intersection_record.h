/*
 * hitrecord.h
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#ifndef INTERSECTION_RECORD_H_
#define INTERSECTION_RECORD_H_

#include <glm/glm.hpp>

#include "spectrum.h"

struct IntersectionRecord
{
    float t_;
    glm::vec3 position_;
    //glm::vec3 normal_;
    Spectrum spectrum_;
};

#endif /* INTERSECTION_RECORD_H_ */
