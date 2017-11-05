#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "bxdf.h"
#include "onb.h"
#include "random.h"
#include "utils.h"

class Lambertian : public BxDF
{
public:

    Lambertian( const glm::vec3 &reflectance,
                SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                Fresnel::FresnelUniquePtr fresnel );

    glm::vec3 fr( const glm::vec3 &w_i,
                   const glm::vec3 &w_r ) const;

    glm::vec3 getNewDirection( const glm::vec3 &w_i ) const;

    glm::vec3 reflectance_;

};

#endif /* LAMBERTIAN_H_ */

