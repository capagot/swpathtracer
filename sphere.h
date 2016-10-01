/*
 * sphere.h
 *
 *  Created on: Sep 28, 2016
 *      Author: christian
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include <glm/glm.hpp>
#include "primitive.h"
#include "ray.h"

class Sphere : public Primitive
{
public:

    Sphere( void );

    Sphere( const glm::vec3 &center,
            float radius,
            const Material *material );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record );

    glm::vec3 center_ = { 0.0f, 0.0f, 0.0f };

    float radius_= 1.0f;

private:

    // TODO: this is replicated for each primitive... avoid that!
    static const float kIntersectionTestEpsilon_;
};

#endif /* SPHERE_H_ */
