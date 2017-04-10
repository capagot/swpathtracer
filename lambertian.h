#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "bxdf.h"
#include "onb.h"
#include "random.h"

class Lambertian : public BxDF
{
public:

    Lambertian( const glm::dvec3 &radiance,
                SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler );

    glm::dvec3 fr( const glm::dvec3 &w_i,
                   const glm::dvec3 &w_r ) const;

    glm::dvec3 getNewDirection( const glm::dvec3 &w_i ) const;

    glm::dvec3 radiance_;

};

#endif /* LAMBERTIAN_H_ */
