#ifndef SMOOTH_TRIANGLE_H_
#define SMOOTH_TRIANGLE_H_

#include <glm/glm.hpp>
#include "primitive.h"
#include "ray.h"

class SmoothTriangle : public Primitive
{
public:

    SmoothTriangle( void );

    SmoothTriangle( const glm::dvec3 &v0,
                    const glm::dvec3 &v1,
                    const glm::dvec3 &v2,
                    const glm::dvec3 &n0,
                    const glm::dvec3 &n1,
                    const glm::dvec3 &n2,
                    long unsigned int material_id );

    bool intersect( const Ray &ray,
                    IntersectionRecord &intersection_record );

    AABB getAABB( void ) const;

    virtual void printData( void ) const;

    glm::dvec3 v0_;

    glm::dvec3 v1_;

    glm::dvec3 v2_;

    glm::dvec3 n0_;

    glm::dvec3 n1_;

    glm::dvec3 n2_;

private:

    static const double kIntersectionTestEpsilon_;
};

#endif /* SMOOTH_TRIANGLE_H_ */

