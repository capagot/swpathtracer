#ifndef INTEGRATORS_DEPTH_RAYCASTER_H
#define INTEGRATORS_DEPTH_RAYCASTER_H

#include "raycaster.h"
#include "timer.h"

class DepthRayCaster : public RayCaster<float> {
   public:
    DepthRayCaster(Camera& camera, const Scene& scene, float prescribed_min_depth, float prescribed_max_depth);
    void saveImageToFile() override;

   private:
    float getHitValue(const IntersectionRecord& intersection_record, std::size_t num_intersection_tests,
                      std::size_t num_intersections) const override;
    float getMissValue(std::size_t num_intersection_tests) const override;

    float prescribed_min_depth_;
    float prescribed_max_depth_;
    float min_depth_;
    float max_depth_;
};

#endif  // INTEGRATORS_DEPTH_RAYCASTER_H
