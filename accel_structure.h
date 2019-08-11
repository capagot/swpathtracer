#ifndef ACCEL_STRUCTURE_H
#define ACCEL_STRUCTURE_H

#include <memory>
#include <vector>

#include "intersection_record.h"
#include "primitive.h"

struct Stats {
    long int build_time_ = 0;
    std::size_t num_references_ = 0;
};

class AccelStructure {
   public:
    enum class Type { NONE, BVH_SAH, SBVH_SAH };

    AccelStructure(Type type) : type_(type) {}
    virtual ~AccelStructure(){};
    inline Type getType() const {
        return type_;
    }
    inline void attachPrimitiveList(const std::vector<std::unique_ptr<Primitive>>* primitives) {
        primitives_ = primitives;
    }
    virtual void build() = 0;
    virtual const Stats& getStatistics() const = 0;
    virtual bool intersect(const Ray& ray, IntersectionRecord& intersection_record, std::size_t& num_intersection_tests,
                           std::size_t& num_intersections) const = 0;

   protected:
    Type type_;
    const std::vector<std::unique_ptr<Primitive>>* primitives_;
};

#endif  // ACCEL_STRUCTURE_H
