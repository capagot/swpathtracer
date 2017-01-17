#ifndef BRDF_H_
#define BRDF_H_

#include <memory>

#include "surface_sampler.h"

class BRDF
{
public:

    typedef std::unique_ptr< BRDF > BRDFUniquePtr;

    // TODO: call custom BRDF constructor in derived classes and remove this default constructor!
    //BRDF( void )
    //{};

    BRDF( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler ) :
          surface_sampler_( std::move( surface_sampler ) )
    {};

    virtual glm::dvec3 fr( const glm::dvec3 &w_i,
                           const glm::dvec3 &w_r ) const = 0;

    virtual glm::dvec3 getNewDirection( const glm::dvec3 &w_i,
                                        const glm::dvec3 &normal,
                                        RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) const = 0;

    SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler_;

};

#endif /* BRDF_H_ */
