#ifndef INTERSECTION_TEST_COUNT_RAYCASTER_H
#define INTERSECTION_TEST_COUNT_RAYCASTER_H

#include "raycaster.h"
#include "timer.h"

class IntersectionTestCountRayCaster : public RayCaster<long int> {
   public:
    IntersectionTestCountRayCaster(Camera& camera, const Scene& scene, long int prescribed_min_int_tests_count,
                                   long int prescribed_max_int_tests_count);
    void saveImageToFile();

   private:
    long int getHitValue(const IntersectionRecord& intersection_record, std::size_t num_intersection_tests,
                         std::size_t num_intersections) const;
    long int getMissValue(std::size_t num_intersection_tests) const;
    float Hue2RGB(float v1, float v2, float h) const;

    long int prescribed_min_int_tests_count_;
    long int prescribed_max_int_tests_count_;
};

#endif  // INTERSECTION_TESTS_COUNT_RAYCASTER_H
