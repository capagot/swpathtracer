#ifndef NORMAL_RAYCASTER_H
#define NORMAL_RAYCASTER_H

#include "raycaster.h"
#include "timer.h"

class NormalRayCaster : public RayCaster<glm::vec3> {
   public:
    NormalRayCaster(Camera& camera, const Scene& scene);
    void saveImageToFile();

   private:
    glm::vec3 getHitValue(const IntersectionRecord& intersection_record, std::size_t num_intersection_tests,
                          std::size_t num_intersections) const;
    glm::vec3 getMissValue(std::size_t num_intersection_tests) const;
};

#endif  // NORMAL_RAYCASTER_H
