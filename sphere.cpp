#include "sphere.h"

Sphere::Sphere( void )
{}

Sphere::Sphere( const glm::vec3 &center,
                float radius,
                long unsigned int material_id ) :
        Primitive{ material_id },
        center_{ center },
        radius_{ radius }
{}

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

    float t1;
    float t2;

    glm::vec3 eo = center_ - ray.origin_;
    float v = glm::dot( eo, ray.direction_ );
    float disc = ( radius_ * radius_ ) - ( glm::dot( eo, eo ) - ( v * v ) );

    if ( disc < 0.0f )
        return false;                           // no intersection
    else
    {
        float d = sqrtf( disc );
        t1 = v - d;                             // first intersection point
        t2 = v + d;                             // second intersection point
    }

    // TODO: add reference to the intersected primitive
    intersection_record.t_ =  ( t1 > 0.00001f ) ? t1 : t2;
    intersection_record.position_ = ray.origin_ + intersection_record.t_ * ray.direction_;
    intersection_record.normal_ = glm::normalize( intersection_record.position_ - center_ );
    intersection_record.material_id_ = material_id_;

    return true;
}

/*AABB Sphere::computeSBVHAABB(float p0, float p1, int axis) {
    return AABB(glm::vec3(0.0f, 0.0f, 0.0), glm::vec3(0.0f, 0.0f, 0.0));
}
*/

bool Sphere::computeSBVHAABB(float min_x,
                             float max_x,
                             float min_y,
                             float max_y,
                             float min_z,
                             float max_z,
                             int axis,
                             AABB& aabb,
                             glm::vec3& centroid) {
    return false;
}

AABB Sphere::getAABB( void ) const
{
    AABB aabb;

    aabb.min_ = center_ - radius_;
    aabb.max_ = center_ + radius_;
    aabb.centroid_ = center_;

    return aabb;
}

glm::vec3 Sphere::getCentroid( void ) const {
    return center_;
}

void Sphere::printData( void ) const
{
    std::cerr << "radius: " << radius_ << " ";
    std::cerr << "center: " << center_.x << ", "
                            << center_.y << ", "
                            << center_.z << "\n";
}
