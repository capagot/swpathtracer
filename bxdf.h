#ifndef BXDF_H_
#define BXDF_H_

#include <memory>

#include "surface_sampler.h"

class BxDF
{
public:

    using BxDFUniquePtr = std::unique_ptr< BxDF >;

    BxDF( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler ) :
        surface_sampler_( std::move( surface_sampler ) )
    {};

    virtual glm::dvec3 fr( const glm::dvec3 &w_i,
                           const glm::dvec3 &w_r ) const = 0;

    virtual glm::dvec3 getNewDirection( const glm::dvec3 &w_i ) const = 0;

//private:

    SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler_;

};

#endif /* BXDF_H_ */
