#include "uniform_pixel_sampler.h"

UniformPixelSampler::UniformPixelSampler(std::size_t spp,
                                         PRNG<std::uniform_real_distribution, float, std::mt19937> &prng)
    : PixelSampler(PixelSampler::Type::UNIFORM, spp), prng_(prng) {}

glm::vec2 UniformPixelSampler::getSample(unsigned int sample_id) {
    (void)sample_id;
    return glm::vec2(prng_(), prng_());
}
