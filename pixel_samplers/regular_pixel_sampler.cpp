#include "regular_pixel_sampler.h"

RegularPixelSampler::RegularPixelSampler(std::size_t spp) : PixelSampler(PixelSampler::Type::REGULAR) {
    unsigned int num_subpixels_dim = static_cast<unsigned int>(std::ceil(std::sqrt(spp)));
    spp_ = num_subpixels_dim * num_subpixels_dim;
    float subpixel_width = 1.0f / num_subpixels_dim;
    float subpixel_offset = subpixel_width * 0.5f;
    samples_.reserve(spp_);

    for (unsigned int i = 0; i < num_subpixels_dim; ++i)
        for (unsigned int j = 0; j < num_subpixels_dim; ++j) {
            samples_.push_back(glm::vec2(subpixel_width * i + subpixel_offset, subpixel_width * j + subpixel_offset));
        }
}

glm::vec2 RegularPixelSampler::getSample(unsigned int sample_id) {
    assert(sample_id < samples_.size());
    return samples_[sample_id];
}
