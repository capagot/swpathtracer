#ifndef BSDFS_BSDF_H
#define BSDFS_BSDF_H

#include <glm/vec3.hpp>
#include <memory>

#include "bsdf_sample.h"

class BSDF {
   public:
    enum class Type { NONE, OPAQUE, TRANSMISSIVE };

    BSDF(Type type);
    virtual ~BSDF() = 0;
    virtual glm::vec3 f(const glm::vec3& local_wo, const glm::vec3& local_wi) const = 0;
    virtual glm::vec3 getReflectance(const glm::vec3& local_wo, const glm::vec3& local_wi) const = 0;
    virtual BSDFSample getSample(const glm::vec3& local_wo) const = 0;
    inline Type getType() const {
        return type_;
    }

   private:
    Type type_;
};

#endif  // BSDFS_BSDF_H
