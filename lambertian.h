#ifndef LAMBERTIAN_H_
#define LAMBERTIAN_H_

#include "onb.h"
#include "random.h"
#include "brdf.h"

class Lambertian : public BRDF
{
public:

    Lambertian( const glm::dvec3 &radiance,
                SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler );

    glm::dvec3 fr( const glm::dvec3 &w_i,
                   const glm::dvec3 &w_r ) const
    {
        ( void ) w_i; // unused variable

        return ( 1.0 / surface_sampler_->getProbability( w_i, w_r ) ) * ( radiance_ / M_PI ) *  w_r.y;
    }

    glm::dvec3 getNewDirection( const glm::dvec3 &w_i,
                                const glm::dvec3 &normal,
                                RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) const;

    glm::dvec3 radiance_ = glm::dvec3{ 0.5, 0.5, 0.5 };

};

#endif /* LAMBERTIAN_H_ */
