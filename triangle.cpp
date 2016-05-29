/*
 * triangle.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#include "triangle.h"

const float Triangle::intersection_test_epsilon = 0.000001f;

Triangle::Triangle( const glm::vec3 &v0,
                    const glm::vec3 &v1,
                    const glm::vec3 &v2,
                    const Spectrum &spectrum ) :
        Primitive{ spectrum },
        v0_{ v0 },
        v1_{ v1 },
        v2_{ v2 }
{ }

bool Triangle::intersect( const Ray &ray,
                          IntersectionRecord &intersection_record  )
{
    /* Ray-triangle intersection test based on the very efficient algorithm presented in the paper:
     *
     *     "Fast, Minimum Storage Ray/Triangle Intersection".
     *     Tomas Moller and Ben Trumbore.
     *     Journal of Graphics Tools.
     *     Volume 2 Issue 1, 1997.
     *     Pages 21-28
     */

    glm::vec3 edge1 = v1_ - v0_;
    glm::vec3 edge2 = v2_ - v0_;
    glm::vec3 pvec = glm::cross( ray.direction_, edge2 );

    float det = glm::dot( edge1, pvec );

#define CULL_TEST

#ifdef CULL_TEST // culling branch

    if ( det < Triangle::intersection_test_epsilon )
        return false;

    glm::vec3 tvec{ ray.origin_ - v0_ };

    float u = glm::dot( tvec, pvec );

    if ( u < 0.0f || u > det )
        return false;

    glm::vec3 qvec{ glm::cross( tvec, edge1 ) };

    float v = glm::dot( ray.direction_, qvec );

    if ( v < 0.0f || ( u + v ) > det )
        return false;

    float t = glm::dot( edge2, qvec );

    float inv_det = 1.0f / det;

    t *= inv_det;
    u *= inv_det;
    v *= inv_det;

#else // non-culling branch

    if ( det > -Triangle::intersection_test_epsilon && det < Triangle::intersection_test_epsilon )
        return false;

    float inv_det = 1.0f / det;

    glm::vec3 tvec{ ray.origin_ - v0_ };

    float u = glm::dot( tvec, pvec ) * inv_det;

    if ( u < 0.0f || u > 1.0 )
        return false;

    glm::vec3 qvec{ glm::cross( tvec, edge1 ) };

    float v = glm::dot( ray.direction_, qvec ) * inv_det;

    if ( v < 0.0f || ( u + v ) > 1.0 )
        return false;

    float t = glm::dot( edge2, qvec ) * inv_det;
#endif

    intersection_record.t_ = t;
    intersection_record.position_ = ray.origin_ + ray.direction_ * t;
    intersection_record.spectrum_ = spectrum_;

    return true;
}
