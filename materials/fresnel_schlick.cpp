#include "fresnel_schlick.h"

FresnelSchlick::FresnelSchlick(float ior_i, float ior_t)
    : Fresnel(Fresnel::Type::SCHLICK), schlick_type_(FresnelSchlick::SchlickType::IOR), ior_i_(ior_i), ior_t_(ior_t) {}

FresnelSchlick::FresnelSchlick(const glm::vec3& reflectance_normal_incidence)
    : Fresnel(Fresnel::Type::SCHLICK),
      schlick_type_(FresnelSchlick::SchlickType::REFLECTANCE_AT_NORMAL_INCIDENCE),
      reflectance_(reflectance_normal_incidence) {}

glm::vec3 FresnelSchlick::getReflectance(float cos_theta_wo) const {
    float cos_th_wo_5;

    if (schlick_type_ == FresnelSchlick::SchlickType::REFLECTANCE_AT_NORMAL_INCIDENCE) {
        cos_th_wo_5 = 1.0f - cos_theta_wo;
        cos_th_wo_5 = cos_th_wo_5 * cos_th_wo_5 * cos_th_wo_5 * cos_th_wo_5 * cos_th_wo_5;
        return reflectance_ + (1.0f - reflectance_) * cos_th_wo_5;
    }

    // local_wo is entering the object (same side as the normal vector)
    float ior_i = ior_i_;
    float ior_t = ior_t_;

    if (cos_theta_wo < 0.0f) {  // local_wo is leaving the object (opposite side with respect to the normal vector)
        ior_i = ior_t_;
        ior_t = ior_i_;
        float eta_ratio = ior_i / ior_t;
        float a = 1 - eta_ratio * eta_ratio * (1.0f - cos_theta_wo * cos_theta_wo);
        cos_theta_wo = sqrt(a);  // cosine of the transmitted ray
    }

    float r0 = (ior_i - ior_t) / (ior_i + ior_t);
    r0 = r0 * r0;

    cos_th_wo_5 = 1.0f - cos_theta_wo;
    cos_th_wo_5 = cos_th_wo_5 * cos_th_wo_5 * cos_th_wo_5 * cos_th_wo_5 * cos_th_wo_5;

    return glm::vec3(r0 + (1.0f - r0) * cos_th_wo_5);
}
