#include "triangle.h"

const float Triangle::kIntersectionTestEpsilon_ = 1.0e-6f;

Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, long unsigned int material_id)
    : Primitive(material_id), v_{v0, v1, v2}, normal_{glm::normalize(glm::cross(v_[1] - v_[0], v_[2] - v_[0]))} {}

Triangle::Triangle(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& normal,
                   long unsigned int material_id)
    : Primitive(material_id), v_{v0, v1, v2}, normal_{normal} {}

bool Triangle::intersect(const Ray& ray, IntersectionRecord& intersection_record) {
    /* Ray-triangle intersection test based on the algorithm presented in:
     *
     *     "Fast, Minimum Storage Ray/Triangle Intersection".
     *     Tomas Moller and Ben Trumbore.
     *     Journal of Graphics Tools.
     *     Volume 2 Issue 1, 1997.
     *     Pages 21-28
     */

    glm::vec3 edge1 = v_[1] - v_[0];
    glm::vec3 edge2 = v_[2] - v_[0];
    glm::vec3 pvec = glm::cross(ray.getDirection(), edge2);
    float det = glm::dot(edge1, pvec);

    //#define CULL_TEST

#ifdef CULL_TEST  // culling branch

    if (det < Triangle::kIntersectionTestEpsilon_) return false;

    glm::vec3 tvec{ray.getOrigin() - v_[0]};
    float u = glm::dot(tvec, pvec);

    if ((u < 0.0f) || (u > det)) return false;

    glm::vec3 qvec{glm::cross(tvec, edge1)};
    float v = glm::dot(ray.getDirection(), qvec);

    if ((v < 0.0f) || ((u + v) > det)) return false;

    float t = glm::dot(edge2, qvec);
    float inv_det = 1.0f / det;

    t *= inv_det;
    u *= inv_det;
    v *= inv_det;

#else  // non-culling branch

    if ((det > -Triangle::kIntersectionTestEpsilon_) && (det < Triangle::kIntersectionTestEpsilon_)) return false;

    float inv_det = 1.0f / det;
    glm::vec3 tvec{ray.getOrigin() - v_[0]};
    float u = glm::dot(tvec, pvec) * inv_det;

    if ((u < 0.0f) || (u > 1.0f)) return false;

    glm::vec3 qvec{glm::cross(tvec, edge1)};
    float v = glm::dot(ray.getDirection(), qvec) * inv_det;

    if ((v < 0.0f) || ((u + v) > 1.0f)) return false;

    float t = glm::dot(edge2, qvec) * inv_det;
#endif

    intersection_record.t_ = t;
    intersection_record.position_ = ray.getOrigin() + ray.getDirection() * t;
    intersection_record.normal_ = getNormal(u, v);
    intersection_record.material_id_ = material_id_;

    return true;
}

AABB Triangle::getAABB() const {
    AABB aabb;

    aabb.min_ = glm::min(glm::min(v_[0], v_[1]), v_[2]);
    aabb.max_ = glm::max(glm::max(v_[0], v_[1]), v_[2]);
    aabb.centroid_ = (1.0f / 3.0f) * (v_[0] + v_[1] + v_[2]);
    return aabb;
}

glm::vec3 Triangle::getCentroid() const {
    return (v_[0] + v_[1] + v_[2]) * (1.0f / 3.0f);
}

Triangle::PointPosition Triangle::classifyPointToPlane(const glm::vec3& point, const Plane& plane) {
    float plane_thickness = 1.0e-3;
    float dist = glm::dot(plane.normal_, point) - glm::dot(plane.normal_, plane.point_);
    if (dist > plane_thickness) return PointPosition::IN_FRONT;  // point in front
    if (dist < -plane_thickness) return PointPosition::BEHIND;   // point behind
    return PointPosition::ON_PLANE;                              // point on plne
}

glm::vec3 Triangle::intersectEdgeAgainstPlane(const glm::vec3& point_a, const glm::vec3& point_b, const Plane& plane) {
    glm::vec3 d = glm::normalize(point_b - point_a);
    float d_dot_n = glm::dot(d, plane.normal_);
    float t = glm::dot((plane.point_ - point_a), plane.normal_) / d_dot_n;
    return t * d + point_a;
}

void Triangle::splitPolygon(const std::vector<glm::vec3>& vertex_list, const Plane& plane,
                            std::vector<glm::vec3>& new_vertex_list) {
    if (vertex_list.size() > 0) {
        glm::vec3 point_a = vertex_list[vertex_list.size() - 1];  // starts with the last vertex in the list
        PointPosition a_side = classifyPointToPlane(point_a, plane);

        for (const glm::vec3& point_b : vertex_list) {
            PointPosition b_side = classifyPointToPlane(point_b, plane);
            if (b_side == PointPosition::IN_FRONT) {
                if (a_side == PointPosition::BEHIND) {
                    glm::vec3 point_i = intersectEdgeAgainstPlane(point_a, point_b, plane);
                    new_vertex_list.push_back(point_i);
                }
            } else if (b_side == PointPosition::BEHIND) {
                if (a_side == PointPosition::IN_FRONT) {
                    glm::vec3 point_i = intersectEdgeAgainstPlane(point_a, point_b, plane);
                    new_vertex_list.push_back(point_i);
                } else if (a_side == PointPosition::ON_PLANE)
                    new_vertex_list.push_back(point_a);
                new_vertex_list.push_back(point_b);
            } else if (a_side == PointPosition::BEHIND)
                new_vertex_list.push_back(point_b);
            else if (a_side == PointPosition::ON_PLANE)
                new_vertex_list.push_back(point_a);

            point_a = point_b;
            a_side = b_side;
        }
    }
}

bool Triangle::clipAgainstAABB(const AABB& aabb, std::vector<glm::vec3>& new_vertex_list, AABB& new_aabb,
                               glm::vec3& centroid) {
    std::vector<glm::vec3> tmp_vertex_list;
    splitPolygon(v_, Plane(glm::vec3(-1.0f, 0.0f, 0.0f), aabb.min_), tmp_vertex_list);
    splitPolygon(tmp_vertex_list, Plane(glm::vec3(0.0f, -1.0f, 0.0f), aabb.min_), new_vertex_list);
    tmp_vertex_list.clear();
    splitPolygon(new_vertex_list, Plane(glm::vec3(0.0f, 0.0f, -1.0f), aabb.min_), tmp_vertex_list);
    new_vertex_list.clear();
    splitPolygon(tmp_vertex_list, Plane(glm::vec3(1.0f, 0.0f, 0.0f), aabb.max_), new_vertex_list);
    tmp_vertex_list.clear();
    splitPolygon(new_vertex_list, Plane(glm::vec3(0.0f, 1.0f, 0.0f), aabb.max_), tmp_vertex_list);
    new_vertex_list.clear();
    splitPolygon(tmp_vertex_list, Plane(glm::vec3(0.0f, 0.0f, 1.0f), aabb.max_), new_vertex_list);

    if (new_vertex_list.size() < 3) return false;

    // This is not actually the centroid of the polygon, but the center of mass of the vertices, which
    // is a point that will be certainly inside the polygon... and this is enough for the polygon ordering
    // during spatial split evaluation.
    centroid = glm::vec3(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0; i < new_vertex_list.size(); ++i) {
        centroid += new_vertex_list[i];

        if (i == 0) {
            new_aabb.min_ = new_vertex_list[i];
            new_aabb.max_ = new_vertex_list[i];
        } else {
            new_aabb.min_ = glm::min(new_aabb.min_, new_vertex_list[i]);
            new_aabb.max_ = glm::max(new_aabb.max_, new_vertex_list[i]);
        }
    }

    centroid /= new_vertex_list.size();

    return true;
}
