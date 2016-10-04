#include "triangle.h"

const double Triangle::kIntersectionTestEpsilon_ = 0.000001;

Triangle::Triangle( void )
{ }

Triangle::Triangle( const glm::dvec3 &v0,
                    const glm::dvec3 &v1,
                    const glm::dvec3 &v2,
                    const Material *material ) :
        Primitive{ material },
        v0_{ v0 },
        v1_{ v1 },
        v2_{ v2 },
        normal_{ glm::normalize( glm::cross( v1_ - v0_, v2_ - v0_ ) ) }
{ }

Triangle::Triangle( const glm::dvec3 &v0,
                    const glm::dvec3 &v1,
                    const glm::dvec3 &v2,
                    const glm::dvec3 &normal,
                    const Material *material ) :
        Primitive{ material },
        v0_{ v0 },
        v1_{ v1 },
        v2_{ v2 },
        normal_{ normal }
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

    glm::dvec3 edge1 = v1_ - v0_;
    glm::dvec3 edge2 = v2_ - v0_;
    glm::dvec3 pvec = glm::cross( ray.direction_, edge2 );

    double det = glm::dot( edge1, pvec );

//#define CULL_TEST

#ifdef CULL_TEST // culling branch

    if ( det < Triangle::kIntersectionTestEpsilon_ )
        return false;

    glm::dvec3 tvec{ ray.origin_ - v0_ };

    double u = glm::dot( tvec, pvec );

    if ( ( u < 0.0 ) || ( u > det ) )
        return false;

    glm::dvec3 qvec{ glm::cross( tvec, edge1 ) };

    double v = glm::dot( ray.direction_, qvec );

    if ( ( v < 0.0 ) || ( ( u + v ) > det ) )
        return false;

    double t = glm::dot( edge2, qvec );

    double inv_det = 1.0 / det;

    t *= inv_det;
    u *= inv_det;
    v *= inv_det;

#else // non-culling branch

    if ( ( det > -Triangle::kIntersectionTestEpsilon_ ) && ( det < Triangle::kIntersectionTestEpsilon_ ) )
        return false;

    double inv_det = 1.0 / det;

    glm::dvec3 tvec{ ray.origin_ - v0_ };

    double u = glm::dot( tvec, pvec ) * inv_det;

    if ( ( u < 0.0 ) || ( u > 1.0 ) )
        return false;

    glm::dvec3 qvec{ glm::cross( tvec, edge1 ) };

    double v = glm::dot( ray.direction_, qvec ) * inv_det;

    if ( ( v < 0.0 ) || ( ( u + v ) > 1.0 ) )
        return false;

    double t = glm::dot( edge2, qvec ) * inv_det;
#endif

    intersection_record.t_ = t;
    intersection_record.position_ = ray.origin_ + ray.direction_ * t;
    intersection_record.normal_ = normal_;
    intersection_record.material_ = material_;

    return true;
}

