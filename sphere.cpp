#include "sphere.h"

Sphere::Sphere(const glm::vec3& center, float radius, long unsigned int material_id)
    : Primitive(material_id), center_(center), radius_(radius) {}

bool Sphere::intersect(const Ray& ray, IntersectionRecord& intersection_record) {
    /* Efficient ray-sphere intersection algorithm presented in the article:
     *
     *     "Intersection of a Ray with a Sphere".
     *     Jeff Hultquist.
     *     Graphics Gems.
     *     Academic Press.
     *     1990.
     */

    float t1;
    float t2;

    glm::vec3 eo = center_ - ray.getOrigin();
    float v = glm::dot(eo, ray.getDirection());
    float disc = (radius_ * radius_) - (glm::dot(eo, eo) - (v * v));

    if (disc < 0.0f)
        return false;  // no intersection
    else {
        float d = sqrtf(disc);
        t1 = v - d;  // first intersection point
        t2 = v + d;  // second intersection point
    }

    // TODO: add reference to the intersected primitive
    intersection_record.t_ = (t1 > 0.00001f) ? t1 : t2;
    intersection_record.position_ = ray.getOrigin() + intersection_record.t_ * ray.getDirection();
    intersection_record.normal_ = glm::normalize(intersection_record.position_ - center_);
    intersection_record.material_id_ = material_id_;

    return true;
}

bool Sphere::clipAgainstAABB(const AABB& aabb,
                     std::vector<glm::vec3>& new_vertex_list,
                     AABB& new_aabb,
                     glm::vec3& centroid) {
    (void)new_vertex_list;
    (void)centroid;

    return getAABB().getIntersection(aabb, new_aabb);
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
