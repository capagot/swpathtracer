#ifndef PIXEL_SAMPLERS_JITTERED_PIXEL_SAMPLER_H
#define PIXEL_SAMPLERS_JITTERED_PIXEL_SAMPLER_H

#include "pixel_sampler.h"
#include "prng.h"

class JitteredPixelSampler : public PixelSampler {
   public:
    JitteredPixelSampler(std::size_t spp, PRNG<std::uniform_real_distribution, float, std::mt19937>& prng);
    glm::vec2 getSample(unsigned int sample_id) override;

   private:
    std::vector<glm::vec2> samples_;
    float subpixel_width_;
    PRNG<std::uniform_real_distribution, float, std::mt19937>& prng_;
};

#endif  // PIXEL_SAMPLERS_JITTERED_PIXEL_SAMPLER_H
