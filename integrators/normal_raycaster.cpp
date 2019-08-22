#include "normal_raycaster.h"

NormalRayCaster::NormalRayCaster(Camera& camera, const Scene& scene)
    : RayCaster(camera, scene, Integrator::Type::NORMAL_RAYCASTER) {}

void NormalRayCaster::saveImageToFile() {
    for (unsigned int x = 0; x < camera_.getImageBuffer().getImageWidth(); ++x)
        for (unsigned int y = 0; y < camera_.getImageBuffer().getImageHeight(); ++y) {
            if ((buffer_[x][y][0] != 0.0f) || (buffer_[x][y][1] != 0.0f) ||
                (buffer_[x][y][2] != 0.0f))  // if it is not background color...

                // normal encoded as: n.xyz * 1/2 + 1/2
                camera_.getImageBuffer().setPixelValue(x, y, buffer_[x][y] * 0.5f + 0.5f);
        }

    camera_.getImageBuffer().saveToFile();
}

glm::vec3 NormalRayCaster::getHitValue(const IntersectionRecord& intersection_record,
                                       std::size_t num_intersection_tests, std::size_t num_intersections) const {
    (void)num_intersection_tests;
    (void)num_intersections;

    return intersection_record.normal_;
}

glm::vec3 NormalRayCaster::getMissValue(std::size_t num_intersection_tests) const {
    (void)num_intersection_tests;

    return glm::vec3(0.0f);
}
