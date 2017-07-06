#ifndef SMOOTH_DIELECTRIC_H_
#define SMOOTH_DIELECTRIC_H_

#include <glm/glm.hpp>

#include "onb.h"
#include "random.h"
#include "bxdf.h"
#include "fresnel.h"

class SmoothDielectric : public BxDF
{
public:

    SmoothDielectric( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                      Fresnel::FresnelUniquePtr fresnel );

    glm::dvec3 fr( const glm::dvec3 &w_i,
                   const glm::dvec3 &w_r ) const;

    glm::dvec3 getNewDirection( const glm::dvec3 &w_i ) const;

};

#endif /* SMOOTH_DIELECTRIC_H_ */
