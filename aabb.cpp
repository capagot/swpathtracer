#include "aabb.h"

float AABB::getArea( void ) const
{
    float l = ( max_.x - min_.x );
    float m = ( max_.y - min_.y );
    float n = ( max_.z - min_.z );
    return 2.0f * ( l * ( m + n ) + m * n );
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

    divx = 1.0f / ray.direction_.x;
    if ( divx >= 0.0f )
    {
        tmin = ( min_.x - ray.origin_.x ) * divx;
        tmax = ( max_.x - ray.origin_.x ) * divx;
    }
    else
    {
        tmin = ( max_.x - ray.origin_.x ) * divx;
        tmax = ( min_.x - ray.origin_.x ) * divx;
    }

    divy = 1.0f / ray.direction_.y;
    if ( divy >= 0.0f )
    {
        tymin = ( min_.y - ray.origin_.y ) * divy;
        tymax = ( max_.y - ray.origin_.y ) * divy;
    }
    else
    {
        tymin = ( max_.y - ray.origin_.y ) * divy;
        tymax = ( min_.y - ray.origin_.y ) * divy;
    }

    if ( ( tmin > tymax ) || ( tymin > tmax ) )
        return false;

    if ( tymin > tmin )
        tmin = tymin;

    if ( tymax < tmax )
        tmax = tymax;

    divz = 1.0f / ray.direction_.z;
    if ( divz >= 0.0f )
    {
        tzmin = ( min_.z - ray.origin_.z ) * divz;
        tzmax = ( max_.z - ray.origin_.z ) * divz;
    }
    else
    {
        tzmin = ( max_.z - ray.origin_.z ) * divz;
        tzmax = ( min_.z - ray.origin_.z ) * divz;
    }

    if ( ( tmin > tzmax ) || ( tzmin > tmax ) )
        return false;

    if ( tzmin > tmin )
        tmin = tzmin;

    if ( tzmax < tmax )
        tmax  = tzmax;

    //intersection_record.t_ = tmax.x;
    //intersection_record.position_ = ray.origin_ + ray.direction_ * t;
    //intersection_record.normal_ = normal_;
    //intersection_record.material_ = material_;

    return true;//tmax > 0.00001f;
}
