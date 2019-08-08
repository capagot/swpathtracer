#ifndef AABB_H_
#define AABB_H_

//#include <iostream>
//#include <iomanip>

#include <fenv.h>
#include <glm/glm.hpp>

#include "core.h"
#include "intersection_record.h"
#include "ray.h"

//#include "primitive.h"

class AABB {
   public:
    AABB(void){};

    AABB(const glm::vec3& min, const glm::vec3& max) : min_{min}, max_{max}, centroid_{0.5f * (min_ + max_)} {};

    ~AABB(void){};

    float getArea(void) const;

    float contains(const AABB& aabb) const;

    bool getIntersection(const AABB& aabb, AABB& result_aabb) const {
        result_aabb = AABB(glm::max(min_, aabb.min_), glm::min(max_, aabb.max_));
        if ((result_aabb.min_.x < result_aabb.max_.x) && (result_aabb.min_.y < result_aabb.max_.y) &&
            (result_aabb.min_.z < result_aabb.max_.z))
            return true;
        return false;
    }

    glm::vec3 getCentroid(void) const;

    bool intersect(const Ray& ray) const;
    // IntersectionRecord &intersection_record,
    // long unsigned int &num_intersection_tests_,
    // long unsigned int &num_intersections_ ) const;

    bool isPointInside(const glm::vec3& point) const {
        const float kInsideTestEpsilon = 1.0e-3f;

        return (point.x >= (min_.x - kInsideTestEpsilon)) && (point.x <= (max_.x + kInsideTestEpsilon)) &&
               (point.y >= (min_.y - kInsideTestEpsilon)) && (point.y <= (max_.y + kInsideTestEpsilon)) &&
               (point.z >= (min_.z - kInsideTestEpsilon)) && (point.z <= (max_.z + kInsideTestEpsilon));
    }

    AABB operator+(const AABB& rhs) const {
        return AABB{glm::min(min_, rhs.min_), glm::max(max_, rhs.max_)};
    }

    glm::vec3 min_;

    glm::vec3 max_;

    glm::vec3 centroid_;
};

#endif /* AABB_H_ */
