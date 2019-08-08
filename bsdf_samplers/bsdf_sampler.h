#ifndef BSDF_SAMPLERS_BSDF_SAMPLER_H
#define BSDF_SAMPLERS_BSDF_SAMPLER_H

#include <glm/vec3.hpp>
#include <iostream>

#include "core.h"
#include "bsdf_sample.h"

class BSDFSampler {
   public:
    BSDFSampler();
    virtual ~BSDFSampler() = 0;
    virtual BSDFSample getSample(const glm::vec3 &local_wo) const = 0;
};

#endif // BSDF_SAMPLERS_BSDF_SAMPLER_H
