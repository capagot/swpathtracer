#ifndef PRIMITIVES_TRIANGLE_H
#define PRIMITIVES_TRIANGLE_H

#include <glm/vec3.hpp>

#include "primitive.h"
#include "ray.h"

class Triangle : public Primitive {
   public:
    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, long unsigned int material_id);
    Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& normal,
             long unsigned int material_id);
    bool intersect(const Ray& ray, IntersectionRecord& intersection_record) override;
    AABB getAABB() const override;
    glm::vec3 getCentroid() const override;
    inline virtual glm::vec3 getNormal(float u, float v) const {
        (void)u;
        (void)v;
        return normal_;
    }
    bool clipAgainstAABB(const AABB& aabb, std::vector<glm::vec3>& new_vertex_list, AABB& new_aabb,
                         glm::vec3& centroid) override;

   private:
    struct Plane {
    public:
        Plane(const glm::vec3 &normal, const glm::vec3 &point) : normal_(normal),
                                                                point_(point) {};
        glm::vec3 normal_;
        glm::vec3 point_;
    };

    enum class PointPosition { IN_FRONT, BEHIND, ON_PLANE };  // used to clip triangle during SBVH construction

    PointPosition classifyPointToPlane(const glm::vec3& point, const Plane& plane);
    glm::vec3 intersectEdgeAgainstPlane(const glm::vec3& point_a, const glm::vec3& point_b, const Plane& plane);
    void splitPolygon(const std::vector<glm::vec3>& vertex_list, const Plane& plane,
                      std::vector<glm::vec3>& new_vertex_list);

    static const float kIntersectionTestEpsilon_;
    std::vector<glm::vec3> v_;
    glm::vec3 normal_;
};

#endif  // PRIMITIVES_TRIANGLE_H
