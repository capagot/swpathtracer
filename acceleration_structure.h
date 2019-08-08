#ifndef ACCELERATION_STRUCTURE_H
#define ACCELERATION_STRUCTURE_H

#include <memory>
#include <vector>

#include "intersection_record.h"
#include "primitive.h"

struct StructureStats {
    long int build_time_ = 0;
    std::size_t num_references_ = 0;
};

class AccelerationStructure {
   public:
    enum class Type { NONE, BVH_SAH, SBVH_SAH };

    AccelerationStructure(Type type) : type_(type) {}
    virtual ~AccelerationStructure(){};
    Type getType() const {
        return type_;
    }
    void attachPrimitiveList(const std::vector<std::unique_ptr<Primitive>>* primitives) {
        primitives_ = primitives;
    }
    virtual void build() = 0;
    virtual const StructureStats& getStatistics() const = 0;
    virtual bool intersect(const Ray& ray, IntersectionRecord& intersection_record, std::size_t& num_intersection_tests,
                           std::size_t& num_intersections) const = 0;

   protected:
    Type type_;
    const std::vector<std::unique_ptr<Primitive>>* primitives_;
};

#endif  // ACCELERATION_STRUCTURE_H
