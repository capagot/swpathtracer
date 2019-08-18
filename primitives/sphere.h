#ifndef PRIMITIVES_SPHERE_H
#define PRIMITIVES_SPHERE_H

#include <glm/vec3.hpp>

#include "primitive.h"
#include "ray.h"

class Sphere : public Primitive {
   public:
    Sphere(const glm::vec3& center, float radius, long unsigned int material_id);
    bool intersect(const Ray& ray, IntersectionRecord& intersection_record) override;
    bool clipAgainstAABB(const AABB& aabb, std::vector<glm::vec3>& new_vertex_list, AABB& new_aabb,
                         glm::vec3& centroid) override;

    AABB getAABB(void) const override;
    glm::vec3 getCentroid(void) const override;

   private:
    glm::vec3 center_;
    float radius_;
    static constexpr float kIntersectionTestEpsilon_ = 1e-5f;
};

#endif  // PRIMITIVES_SPHERE_H
