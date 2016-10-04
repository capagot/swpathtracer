#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <glm/glm.hpp>
#include "primitive.h"
#include "ray.h"

class Triangle : public Primitive
{
public:

    Triangle( void );

    Triangle( const glm::dvec3 &v0,
              const glm::dvec3 &v1,
              const glm::dvec3 &v2,             
              const Material *material );

    Triangle( const glm::dvec3 &v0,
              const glm::dvec3 &v1,
              const glm::dvec3 &v2,
              const glm::dvec3 &normal,
              const Material *material );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record );

    glm::dvec3 v0_ = { 0.0, 0.0, 0.0 };

    glm::dvec3 v1_ = { 1.0, 0.0, 0.0 };

    glm::dvec3 v2_ = { 0.0, 1.0, 0.0 };

    glm::dvec3 normal_ = { 0.0, 0.0, 1.0 };

private:

    static const double kIntersectionTestEpsilon_;
};

#endif /* TRIANGLE_H_ */

