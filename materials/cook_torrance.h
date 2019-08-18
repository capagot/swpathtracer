#ifndef MATERIALS_COOK_TORRANCE_H
#define MATERIALS_COOK_TORRANCE_H

#include <glm/glm.hpp>

#include "bsdf.h"
#include "bsdf_sampler.h"
#include "fresnel.h"

/* The CookTorrance class implements a microfacet-based BRDF model for rough specular surfaces.
 * It implements the rough specular term proposed originally in [1] and includes the normalizing
 * factors proposed in [2]. The Fresnel term currently used is based on a simple and efficient
 * approximation proposed originally in [1].
 *
 *     [1] "A Reflectance Model for Computer Graphics"
 *         Cook, R. L. and Torrance, K. E.
 *         ACM Transactions on Graphics, ACM, 1982, 1, 7-24
 *
 *     [2] "Microfacet Models for Refraction Through Rough Surfaces"
 *         Walter, B.; Marschner, S. R.; Li, H. and Torrance, K. E.
 *         Proceedings of the 18th Eurographics Conference on Rendering Techniques, Eurographics Association, 2007,
 *         195-206.
 */

class CookTorrance : public BSDF {
   public:
    CookTorrance(float roughness, std::unique_ptr<Fresnel> fresnel, std::unique_ptr<BSDFSampler> bsdf_sampler);
    glm::vec3 f(const glm::vec3& local_wo, const glm::vec3& local_wi) const override;
    glm::vec3 getReflectance(const glm::vec3& local_wo, const glm::vec3& local_wi) const override;
    BSDFSample getSample(const glm::vec3& local_wo) const override;

   private:
    std::unique_ptr<BSDFSampler> bsdf_sampler_;
    float roughness_;
    std::unique_ptr<Fresnel> fresnel_;
};

#endif  // MATERIALS_COOK_TORRANCE_H
