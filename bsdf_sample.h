#ifndef BSDF_SAMPLE_H
#define BSDF_SAMPLE_H

#include "glm/vec3.hpp"

struct BSDFSample {
    BSDFSample(const glm::vec3& sample, float probability) : sample_(sample), probability_(probability) {}

    glm::vec3 sample_;
    float probability_;
};

#endif  // BSDF_SAMPLE_H
