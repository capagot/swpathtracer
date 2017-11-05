#include "smooth_triangle.h"

const float SmoothTriangle::kIntersectionTestEpsilon_ = 0.000001f;

SmoothTriangle::SmoothTriangle( void )
{}

SmoothTriangle::SmoothTriangle( const glm::vec3 &v0,
                                const glm::vec3 &v1,
                                const glm::vec3 &v2,
                                const glm::vec3 &n0,
                                const glm::vec3 &n1,
                                const glm::vec3 &n2,
                                long unsigned int material_id ) :
        Primitive{ material_id },
        v0_{ v0 },
        v1_{ v1 },
        v2_{ v2 },
        n0_{ n0 },
        n1_{ n1 },
        n2_{ n2 }
{}

bool SmoothTriangle::intersect( const Ray &ray,
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

//#define CULL_TEST

#ifdef CULL_TEST // culling branch

    if ( det < Triangle::kIntersectionTestEpsilon_ )
        return false;

    glm::vec3 tvec{ ray.origin_ - v0_ };

    float u = glm::dot( tvec, pvec );

    if ( ( u < 0.0f ) || ( u > det ) )
        return false;

    glm::vec3 qvec{ glm::cross( tvec, edge1 ) };

    float v = glm::dot( ray.direction_, qvec );

    if ( ( v < 0.0f ) || ( ( u + v ) > det ) )
        return false;

    float t = glm::dot( edge2, qvec );

    float inv_det = 1.0f / det;

    t *= inv_det;
    u *= inv_det;
    v *= inv_det;

#else // non-culling branch

    if ( ( det > -SmoothTriangle::kIntersectionTestEpsilon_ ) && ( det < SmoothTriangle::kIntersectionTestEpsilon_ ) )
        return false;

    float inv_det = 1.0f / det;

    glm::vec3 tvec{ ray.origin_ - v0_ };

    float u = glm::dot( tvec, pvec ) * inv_det;

    if ( ( u < 0.0f ) || ( u > 1.0f ) )
        return false;

    glm::vec3 qvec{ glm::cross( tvec, edge1 ) };

    float v = glm::dot( ray.direction_, qvec ) * inv_det;

    if ( ( v < 0.0f ) || ( ( u + v ) > 1.0f ) )
        return false;

    float t = glm::dot( edge2, qvec ) * inv_det;
#endif

    intersection_record.t_ = t;
    intersection_record.position_ = ray.origin_ + ray.direction_ * t;
    //intersection_record.normal_ = glm::normalize( n0_ * u + n1_ * v + n2_ * ( 1.0f - u - v ) ); NO
    intersection_record.normal_ = glm::normalize( n0_ * ( 1.0f - u  - v) + n1_ * u + n2_ * v );
    //intersection_record.normal_ = glm::normalize( n0_ *  v + n1_ * ( 1.0f - u  - v) + n2_ * u ); NO
    //intersection_record.normal_ = glm::normalize( n0_ * ( 1.0f - u  - v) + n1_ * v + n2_ * u ); NO

    // DEBUG
    if ( u + v > 1.0f )
        exit( 1 );
 
    intersection_record.material_id_ = material_id_;

    return true;
}

glm::vec3 SmoothTriangle::getUniformPoint( float e1, float e2 ) const
{
    return v0_ + e2 * static_cast< float>( sqrt( 1.0f - e1 ) ) * ( v1_ - v0_ ) + ( 1.0f - static_cast< float >( sqrt( 1.0f - e1 ) ) ) * ( v2_ - v0_ ); 
}

AABB SmoothTriangle::getAABB( void ) const
{
    AABB aabb;

    aabb.min_ = glm::min( glm::min( v0_, v1_ ), v2_ );
    aabb.max_ = glm::max( glm::max( v0_, v1_ ), v2_ );
    aabb.centroid_ = ( 1.0f / 3.0f ) * ( v0_ + v1_ + v2_ );
    return aabb;
}

void SmoothTriangle::printData( void ) const
{}
