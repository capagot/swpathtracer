#include "sphere.h"

Sphere::Sphere( void )
{ }

Sphere::Sphere( const glm::vec3 &center,
        float radius,
        const Material *material ) :
        Primitive{ material },
        center_{ center },
        radius_{ radius }
{ }

bool Sphere::intersect( const Ray &ray,
                        IntersectionRecord &intersection_record  )
{
    /* Ray-sphere intersection test adapted from the very efficient algorithm presented in the article:
     *
     *     "Intersection of a Ray with a Sphere".
     *     Jeff Hultquist.
     *     Graphics Gems.
     *     Academic Press.
     *     1990.
     */

    double t1;
    double t2;

    //glm::vec3 eo = center_ - ray.origin_;
    double eo2[3];
    eo2[0] = static_cast< double >( center_[0] ) - static_cast< double >( ray.origin_[0] );
    eo2[1] = static_cast< double >( center_[1] ) - static_cast< double >( ray.origin_[1] );
    eo2[2] = static_cast< double >( center_[2] ) - static_cast< double >( ray.origin_[2] );

    //double v = glm::dot( eo, ray.direction_ );
    double v = eo2[0] * ray.direction_[0] +
               eo2[1] * ray.direction_[1] +
               eo2[2] * ray.direction_[2];

    //double disc = ( radius_ * radius_ ) - ( glm::dot( eo, eo ) - ( v * v ) );
    double disc = radius_;
    disc *= disc;
    disc -= (eo2[0] * eo2[0]) +
            (eo2[1] * eo2[1]) +
            (eo2[2] * eo2[2]) -
            ( v * v );

    if ( disc < 0.0 )
        return false;                           // no intersection
    else
    {
        double d = sqrt( disc );
        t1 = v - d;                             // first (closer) intersection point
        t2 = v + d;                             // second (farther) intersection point
    }

    intersection_record.t_ =  ( t1 > 0.0001 ) ? t1 : t2;
    intersection_record.position_ = ray.origin_ + intersection_record.t_ * ray.direction_;
    //intersection_record.normal_ = glm::normalize( intersection_record.position_ - center_ );

    glm::dvec3 n;
    n[0] = intersection_record.position_[0] - center_[0];
    n[1] = intersection_record.position_[1] - center_[1];
    n[2] = intersection_record.position_[2] - center_[2];

    n = glm::normalize( n );

    intersection_record.normal_ = glm::vec3{ n[0], n[1], n[2] };
    intersection_record.material_ = material_;

    return true;
}
