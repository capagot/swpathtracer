#ifndef NO_ACCEL_STRUCTURE_H
#define NO_ACCEL_STRUCTURE_H

#include "accel_structure.h"

struct StatsNoAccel : Stats {};

class NoAccelStructure : public AccelStructure {
   public:
    NoAccelStructure();
    ~NoAccelStructure();
    void build() override;
    inline const StatsNoAccel& getStatistics() const override {
        return statistics_;
    }
    bool intersect(const Ray& ray, IntersectionRecord& intersection_record, std::size_t& num_intersection_tests,
                   std::size_t& num_intersections) const override;

   private:
    StatsNoAccel statistics_;
};

#endif  // NO_ACCEL_STRUCTURE_H
