#ifndef BSDF_SAMPLERS_COOK_TORRANCE_BSDF_SAMPLER_H
#define BSDF_SAMPLERS_COOK_TORRANCE_BSDF_SAMPLER_H

#include <math.h>
#include <limits>
#include <glm/glm.hpp>

#include "bsdf_sampler.h"
#include "prng.h"

class CookTorranceBSDFSampler : public BSDFSampler {
   public:
    CookTorranceBSDFSampler(float roughness, PRNG<std::uniform_real_distribution, float, std::mt19937>& prng);
    BSDFSample getSample(const glm::vec3& local_wo) const override;

   private:
    float getProbability(const glm::vec3& local_wo, const glm::vec3& local_wi) const;

    float roughness_;
    PRNG<std::uniform_real_distribution, float, std::mt19937>& prng_;
};

#endif  // BSDF_SAMPLERS_COOK_TORRANCE_BSDF_SAMPLER_H
