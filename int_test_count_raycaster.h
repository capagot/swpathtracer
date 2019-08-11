#ifndef INT_TEST_COUNT_RAYCASTER_H
#define INT_TEST_COUNT_RAYCASTER_H

#include "raycaster.h"
#include "timer.h"

class IntTestCountRayCaster : public RayCaster<long int> {
   public:
    IntTestCountRayCaster(Camera& camera, const Scene& scene, long int prescribed_min_int_tests_count,
                          long int prescribed_max_int_tests_count);
    void saveImageToFile();

   private:
    long int getHitValue(const IntersectionRecord& intersection_record, std::size_t num_int_tests,
                         std::size_t num_ints) const;
    long int getMissValue(std::size_t num_int_tests) const;
    float Hue2RGB(float v1, float v2, float h) const;

    long int prescribed_min_int_tests_count_;
    long int prescribed_max_int_tests_count_;
};

#endif  // INT_TEST_COUNT_RAYCASTER_H
