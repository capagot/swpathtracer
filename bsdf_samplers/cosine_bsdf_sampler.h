#ifndef BSDF_SAMPLERS_COSINE_BSDF_SAMPLER_H
#define BSDF_SAMPLERS_COSINE_BSDF_SAMPLER_H

#include <math.h>

#include "bsdf_sampler.h"
#include "prng.h"

class CosineBSDFSampler : public BSDFSampler {
   public:
    CosineBSDFSampler(PRNG<std::uniform_real_distribution, float, std::mt19937> &prng);
    BSDFSample getSample(const glm::vec3 &local_wo) const override;

   private:
    PRNG<std::uniform_real_distribution, float, std::mt19937> &prng_;
};

#endif  // BSDF_SAMPLERS_COSINE_BSDF_SAMPLER_H
