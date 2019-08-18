#ifndef MATERIALS_FRESNEL_SCHLICK_H
#define MATERIALS_FRESNEL_SCHLICK_H

#include <math.h>

#include "fresnel.h"

class FresnelSchlick : public Fresnel {
   public:
    // A Schlick's fresnel reflectance object can be instantiated
    // in two different ways:
    //
    //  1 - Through the reflectance at normal incidence.
    //  2 - Through two IOR's: one for the external (ior_i_) and one for the transmitting (ior_t_) medium.

    enum class SchlickType { NONE, REFLECTANCE_AT_NORMAL_INCIDENCE, IOR };

    FresnelSchlick(float eta_i, float eta_t);
    FresnelSchlick(const glm::vec3 &reflectance_normal_incidence);
    glm::vec3 getReflectance(float cos_theta_i) const override;

   private:
    SchlickType schlick_type_;
    float ior_i_;
    float ior_t_;
    glm::vec3 reflectance_;
};

#endif  // MATERIALS_FRESNEL_SCHLICK_H
