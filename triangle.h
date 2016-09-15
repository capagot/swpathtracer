/*
 * triangle.h
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <glm/glm.hpp>
#include "primitive.h"
#include "ray.h"
#include "spectrum.h"

class Triangle : public Primitive
{
public:

    Triangle( const glm::vec3 &v0,
              const glm::vec3 &v1,
              const glm::vec3 &v2,             
              const Lambertian &material );

    Triangle( const glm::vec3 &v0,
              const glm::vec3 &v1,
              const glm::vec3 &v2,
              const glm::vec3 &normal,
              const Lambertian &material );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record );

    glm::vec3 v0_ = { 0.0f, 0.0f, 0.0f };

    glm::vec3 v1_ = { 1.0f, 0.0f, 0.0f };

    glm::vec3 v2_ = { 0.0f, 1.0f, 0.0f };

    glm::vec3 normal_ = { 0.0f, 0.0f, 1.0f };

private:

    static const float kIntersectionTestEpsilon_;
};

#endif /* TRIANGLE_H_ */
