#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "bxdf.h"
#include "onb.h"
#include "random.h"

class Lambertian : public BxDF
{
public:

    Lambertian( const glm::dvec3 &reflectance,
                SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                Fresnel::FresnelUniquePtr fresnel );

    glm::dvec3 fr( const glm::dvec3 &w_i,
                   const glm::dvec3 &w_r ) const;

    glm::dvec3 getNewDirection( const glm::dvec3 &w_i ) const;

    glm::dvec3 reflectance_;

};

#endif /* LAMBERTIAN_H_ */

