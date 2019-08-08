#ifndef PIXEL_SAMPLERS_UNIFORM_PIXEL_SAMPLER_H
#define PIXEL_SAMPLERS_UNIFORM_PIXEL_SAMPLER_H

#include "pixel_sampler.h"
#include "prng.h"

class UniformPixelSampler : public PixelSampler {
   public:
    UniformPixelSampler(std::size_t spp, PRNG<std::uniform_real_distribution, float, std::mt19937>& prng);
    glm::vec2 getSample(unsigned int sample_id) override;

   private:
    PRNG<std::uniform_real_distribution, float, std::mt19937>& prng_;
};

#endif  // PIXEL_SAMPLERS_UNIFORM_PIXEL_SAMPLER_H
