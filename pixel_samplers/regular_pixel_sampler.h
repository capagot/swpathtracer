#ifndef PIXEL_SAMPLERS_REGULAR_PIXEL_SAMPLER_H
#define PIXEL_SAMPLERS_REGULAR_PIXEL_SAMPLER_H

#include <cmath>
#include <vector>

#include "pixel_sampler.h"

class RegularPixelSampler : public PixelSampler {
   public:
    RegularPixelSampler(std::size_t spp);
    glm::vec2 getSample(unsigned int sample_id) override;

   private:
    std::vector<glm::vec2> samples_;
};

#endif  // PIXEL_SAMPLERS_REGULAR_PIXEL_SAMPLER_H
