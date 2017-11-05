#ifndef COOK_TORRANCE_H_
#define COOK_TORRANCE_H_

#include "bxdf.h"
#include "onb.h"
#include "random.h"
#include "fresnel.h"
#include "utils.h"

/* The CookTorrance class implements a microfacet-based BRDF model for rough specular surfaces.
 * It implements the rough specular term proposed originally in [1] and includes the normalizing
 * factors proposed in [2]. The Fresnel term currently used is based on a simple (and efficient)
 * approximation proposed originally in [1]. This will be replaced by a more accurate Fresnel term
 * in a future release.
 *
 *     [1] "A Reflectance Model for Computer Graphics"
 *         Cook, R. L. and Torrance, K. E.
 *         ACM Transactions on Graphics, ACM, 1982, 1, 7-24 
 *
 *     [2] "Microfacet Models for Refraction Through Rough Surfaces"
 *         Walter, B.; Marschner, S. R.; Li, H. and Torrance, K. E.
 *         Proceedings of the 18th Eurographics Conference on Rendering Techniques, Eurographics Association, 2007, 195-206.
 */
class CookTorrance : public BxDF
{
public:

    CookTorrance( float m,
                  SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                  Fresnel::FresnelUniquePtr fresnel );

    glm::vec3 fr( const glm::vec3 &w_i,
                   const glm::vec3 &w_r ) const;

    glm::vec3 getNewDirection( const glm::vec3 &w_i ) const;

private:

    float m_; // roughness

};

#endif /* COOK_TORRANCE_H_ */
