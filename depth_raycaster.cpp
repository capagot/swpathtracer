#include "depth_raycaster.h"

DepthRayCaster::DepthRayCaster(Camera& camera, const Scene& scene, float prescribed_min_depth,
                               float prescribed_max_depth)
    : RayCaster(camera, scene, Integrator::Type::DEPTH_RAYCASTER),
      prescribed_min_depth_(prescribed_min_depth),
      prescribed_max_depth_(prescribed_max_depth) {}

void DepthRayCaster::saveImageToFile() {
    min_depth_ = std::numeric_limits<float>::infinity();
    max_depth_ = -std::numeric_limits<float>::infinity();

    for (unsigned int x = 0; x < camera_.getImage().getImageWidth(); ++x)
        for (unsigned int y = 0; y < camera_.getImage().getImageHeight(); ++y) {
            max_depth_ = (max_depth_ < buffer_[x][y]) ? buffer_[x][y] : max_depth_;
            min_depth_ = ((buffer_[x][y] > 0.0f) && (min_depth_ > buffer_[x][y])) ? buffer_[x][y] : min_depth_;
        }

    min_depth_ = (prescribed_min_depth_ != -1.0f) ? prescribed_min_depth_ : min_depth_;
    max_depth_ = (prescribed_max_depth_ != -1.0f) ? prescribed_max_depth_ : max_depth_;
    float depth_range = max_depth_ - min_depth_;

    for (unsigned int x = 0; x < camera_.getImage().getImageWidth(); ++x)
        for (unsigned int y = 0; y < camera_.getImage().getImageHeight(); ++y)
            if (buffer_[x][y] != -1.0f) {  // if it is not background...
                if (depth_range)
                    camera_.getImage().setPixelValue(x, y, glm::vec3((buffer_[x][y] - min_depth_) / depth_range));
                else
                    camera_.getImage().setPixelValue(x, y, glm::vec3(1.0f));
            } else  //  if it is background, paint with black
                camera_.getImage().setPixelValue(x, y, glm::vec3(0.0f));

    camera_.getImage().saveToFile();
}

float DepthRayCaster::getHitValue(const IntersectionRecord& intersection_record, std::size_t num_intersection_tests,
                                  std::size_t num_intersections) const {
    (void)num_intersection_tests;
    (void)num_intersections;

    return intersection_record.t_;
}

float DepthRayCaster::getMissValue(std::size_t num_intersection_tests) const {
    (void)num_intersection_tests;

    return -1.0f;  // just marks pixel as background (i.e. the camera ray has found anything here)
}
