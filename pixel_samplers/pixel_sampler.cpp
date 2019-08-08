#include "pixel_sampler.h"

PixelSampler::PixelSampler(Type type) : spp_(0), type_(type) {}

PixelSampler::PixelSampler(Type type, std::size_t spp) : spp_(spp), type_(type) {}

PixelSampler::~PixelSampler() {}
