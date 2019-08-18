#ifndef ACCELERATION_AABB_H
#define ACCELERATION_AABB_H

#include <fenv.h>
#include <glm/glm.hpp>

#include "core.h"
#include "intersection_record.h"
#include "ray.h"

class AABB {
   public:
    AABB();
    AABB(const glm::vec3& min, const glm::vec3& max);
    ~AABB();
    float getArea() const;
    float contains(const AABB& aabb) const;
    bool getIntersection(const AABB& aabb, AABB& result_aabb) const;
    bool intersect(const Ray& ray) const;
    bool isPointInside(const glm::vec3& point) const;
    inline const glm::vec3& getMin() const {
        return min_;
    }
    inline const glm::vec3& getMax() const {
        return max_;
    }
    inline const glm::vec3& getCentroid() const {
        return centroid_;
    }
    AABB operator+(const AABB& rhs) const {
        return AABB{glm::min(min_, rhs.min_), glm::max(max_, rhs.max_)};
    }
    inline void setMin(const glm::vec3& min) {
        min_ = min;
        updateCentroid();
    }

    inline void setMax(const glm::vec3& max) {
        max_ = max;
        updateCentroid();
    }

   private:
    void updateCentroid() {
        centroid_ = 0.5f * (min_ + max_);
    }

    static const float kInsideTestEpsilon;
    glm::vec3 min_;
    glm::vec3 max_;
    glm::vec3 centroid_;
};

#endif  // ACCELERATION_AABB_H
