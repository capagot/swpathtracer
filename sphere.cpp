#include "sphere.h"

Sphere::Sphere( void )
{ }

Sphere::Sphere( const glm::dvec3 &center,
                double radius,
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

    glm::dvec3 eo = center_ - ray.origin_;
    double v = glm::dot( eo, ray.direction_ );    
    double disc = ( radius_ * radius_ ) - ( glm::dot( eo, eo ) - ( v * v ) );

    if ( disc < 0.0 )
        return false;                           // no intersection
    else
    {
        double d = sqrt( disc );
        t1 = v - d;                             // first intersection point
        t2 = v + d;                             // second intersection point
    }

    intersection_record.t_ =  ( t1 > 0.00001 ) ? t1 : t2;
    intersection_record.position_ = ray.origin_ + intersection_record.t_ * ray.direction_;
    intersection_record.normal_ = glm::normalize( intersection_record.position_ - center_ );
    intersection_record.material_ = material_;

    return true;
}
