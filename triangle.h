#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <glm/vec3.hpp>

#include "plane.h"
#include "primitive.h"
#include "ray.h"

class Triangle : public Primitive {
   public:
    enum class PointPosition { IN_FRONT, BEHIND, ON_PLANE };

    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, long unsigned int material_id);
    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& normal,
             long unsigned int material_id);
    bool intersect(const Ray& ray, IntersectionRecord& intersection_record);
    PointPosition classifyPointToPlane(const glm::vec3& point, const Plane& plane);
    glm::vec3 intersectEdgeAgainstPlane(const glm::vec3& point_a, const glm::vec3& point_b, const Plane& plane);
    void splitPolygon(const std::vector<glm::vec3>& vertex_list, const Plane& plane,
                      std::vector<glm::vec3>& new_vertex_list);
    bool clipAgainstAABB(const AABB& aabb, std::vector<glm::vec3>& new_vertex_list, AABB& new_aabb,
                         glm::vec3& centroid);
    AABB getAABB() const;
    glm::vec3 getCentroid() const;
    virtual glm::vec3 getNormal(float u, float v) {
        (void)u;
        (void)v;
        return normal_;
    }

    std::vector<glm::vec3> v_;
    glm::vec3 normal_ = {0.0f, 0.0f, 1.0f};

   private:
    static const float kIntersectionTestEpsilon_;
};

#endif /* TRIANGLE_H_ */
