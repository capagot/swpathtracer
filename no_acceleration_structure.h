#ifndef NO_ACCELERATION_STRUCTURE_H
#define NO_ACCELERATION_STRUCTURE_H

#include "acceleration_structure.h"

struct StructureStatsNoAccel : StructureStats {};

class NoAccelerationStructure : public AccelerationStructure {
   public:
    NoAccelerationStructure();
    ~NoAccelerationStructure();
    void build();
    const StructureStatsNoAccel& getStatistics() const {
        return statistics_;
    }
    bool intersect(const Ray& ray, IntersectionRecord& intersection_record, std::size_t& num_intersection_tests,
                   std::size_t& num_intersections) const;

   private:
    StructureStatsNoAccel statistics_;
};

#endif  // NO_ACCELERATION_STRUCTURE_H
