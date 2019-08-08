#ifndef SPHERE_H
#define SPHERE_H

#include <glm/vec3.hpp>

#include "primitive.h"
#include "ray.h"

class Sphere : public Primitive {
   public:
    Sphere(const glm::vec3& center, float radius, long unsigned int material_id);
    bool intersect(const Ray& ray, IntersectionRecord& intersection_record);

    // AABB computeSBVHAABB(float p0, float p1, int axis);
    // bool computeSBVHAABB(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, int axis,
    //                      AABB& aabb, glm::vec3& centroid);

    bool clipAgainstAABB(const AABB& aabb, std::vector<glm::vec3>& new_vertex_list, AABB& new_aabb,
                         glm::vec3& centroid);

    AABB getAABB(void) const;

    glm::vec3 getCentroid(void) const;

    // virtual void printData( void ) const;

    glm::vec3 center_;

    float radius_;

   private:

    // TODO: this is replicated for each primitive... avoid that!
    static const float kIntersectionTestEpsilon_;
};

#endif  // SPHERE_H
