#ifndef PIXEL_SAMPLERS_PIXEL_SAMPLER_H
#define PIXEL_SAMPLERS_PIXEL_SAMPLER_H

#include <glm/vec2.hpp>

class PixelSampler {
   public:
    enum class Type { NONE, UNIFORM, REGULAR, JITTERED };

    PixelSampler(Type type);
    PixelSampler(Type type, std::size_t spp);
    virtual ~PixelSampler() = 0;
    virtual glm::vec2 getSample(unsigned int sample_id) = 0;
    inline std::size_t getSPP() const {
        return spp_;
    }
    inline Type getType() const {
        return type_;
    }

   protected:
    std::size_t spp_;
    Type type_;
};

#endif  // PIXEL_SAMPLERS_PIXEL_SAMPLER_H
