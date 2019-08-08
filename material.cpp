#include "material.h"

Material::Material(const LayeredBSDF& layered_bsdf, const glm::vec3& emission)
    : layered_bsdf_(layered_bsdf), emission_(emission) {}

void Material::evalBSDF(const glm::vec3& local_wo, glm::vec3& bsdf, glm::vec3& reflectance, float& pdf, glm::vec3& local_wi) const {
    layered_bsdf_.evalBSDF(local_wo, bsdf, reflectance, pdf, local_wi);
}
