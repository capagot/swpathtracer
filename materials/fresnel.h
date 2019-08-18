#ifndef MATERIALS_FRESNEL_H
#define MATERIALS_FRESNEL_H

#include <glm/vec3.hpp>

class Fresnel {
   public:
    enum class Type { NONE, SCHLICK, DIELECTRIC, COMPLEX };

    Fresnel(Type type);
    virtual ~Fresnel() = 0;
    virtual glm::vec3 getReflectance(float cos_theta_i) const = 0;

   private:
    Type type_;
};

#endif  // MATERIALS_FRESNEL_H
