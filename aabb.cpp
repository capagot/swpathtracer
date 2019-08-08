#include "aabb.h"

float AABB::getArea( void ) const {
    float l = ( max_.x - min_.x );
    float m = ( max_.y - min_.y );
    float n = ( max_.z - min_.z );
    return 2.0f * ( l * ( m + n ) + m * n );
}

float AABB::contains(const AABB& aabb) const {
    return (min_.x <= aabb.min_.x) &&
           (max_.x >= aabb.max_.x) &&
           (min_.y <= aabb.min_.y) &&
           (max_.y >= aabb.max_.y) &&
           (min_.z <= aabb.min_.z) &&
           (max_.z >= aabb.max_.z);
}

glm::vec3 AABB::getCentroid( void ) const {
    return glm::vec3(min_ + max_) * 0.5f;
}

bool AABB::intersect( const Ray &ray ) const
                      //IntersectionRecord &intersection_record,
                      //long unsigned int &num_intersection_tests_,
                      //long unsigned int &num_intersections_ ) const
{
    // An Efficient and Robust Ray–Box Intersection Algorithm,
    //     Amy Williams, Steve Barrus, R. Keith Morley and Peter Shirley

    float tmin;
    float tmax;
    float tymin;
    float tymax;
    float tzmin;
    float tzmax;
    float divx;
    float divy;
    float divz;

    DISABLE_FEEXCEPTION_HANDLING();

    divx = 1.0f / ray.getDirection().x;
    if ( divx >= 0.0f )
    {
        tmin = ( min_.x - ray.getOrigin().x ) * divx;
        tmax = ( max_.x - ray.getOrigin().x ) * divx;
    }
    else
    {
        tmin = ( max_.x - ray.getOrigin().x ) * divx;
        tmax = ( min_.x - ray.getOrigin().x ) * divx;
    }

    divy = 1.0f / ray.getDirection().y;
    if ( divy >= 0.0f )
    {
        tymin = ( min_.y - ray.getOrigin().y ) * divy;
        tymax = ( max_.y - ray.getOrigin().y ) * divy;
    }
    else
    {
        tymin = ( max_.y - ray.getOrigin().y ) * divy;
        tymax = ( min_.y - ray.getOrigin().y ) * divy;
    }

    if ( ( tmin > tymax ) || ( tymin > tmax ) ) {
        ENABLE_FEEXCEPTION_HANDLING();
        return false;
    }

    if ( tymin > tmin )
        tmin = tymin;

    if ( tymax < tmax )
        tmax = tymax;

    divz = 1.0f / ray.getDirection().z;
    if ( divz >= 0.0f )
    {
        tzmin = ( min_.z - ray.getOrigin().z ) * divz;
        tzmax = ( max_.z - ray.getOrigin().z ) * divz;
    }
    else
    {
        tzmin = ( max_.z - ray.getOrigin().z ) * divz;
        tzmax = ( min_.z - ray.getOrigin().z ) * divz;
    }

    if ( ( tmin > tzmax ) || ( tzmin > tmax ) ) {
        ENABLE_FEEXCEPTION_HANDLING();
        return false;
    }

    if ( tzmin > tmin )
        tmin = tzmin;

    if ( tzmax < tmax )
        tmax  = tzmax;

    //intersection_record.t_ = tmax.x;
    //intersection_record.position_ = ray.getOrigin() + ray.getDirection() * t;
    //intersection_record.normal_ = normal_;
    //intersection_record.material_ = material_;

    ENABLE_FEEXCEPTION_HANDLING();
    return true;//tmax > 0.00001f;
}
