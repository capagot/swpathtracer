#include "jittered_pixel_sampler.h"

JitteredPixelSampler::JitteredPixelSampler(std::size_t spp,
                                           PRNG<std::uniform_real_distribution, float, std::mt19937>& prng)
    : PixelSampler(PixelSampler::Type::JITTERED, 0), prng_(prng) {
    unsigned int num_subpixels_dim = static_cast<unsigned int>(std::ceil(std::sqrt(spp)));
    spp_ = num_subpixels_dim * num_subpixels_dim;
    subpixel_width_ = 1.0f / num_subpixels_dim;
    samples_.reserve(spp_);

    for (unsigned int i = 0; i < num_subpixels_dim; ++i)
        for (unsigned int j = 0; j < num_subpixels_dim; ++j) {
            samples_.push_back(glm::vec2(subpixel_width_ * i, subpixel_width_ * j));
        }
}

glm::vec2 JitteredPixelSampler::getSample(unsigned int sample_id) {
    assert(sample_id < samples_.size());
    return samples_[sample_id] + glm::vec2(subpixel_width_ * prng_(), subpixel_width_ * prng_());
}
