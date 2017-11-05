#ifndef BXDF_H_
#define BXDF_H_

#include <memory>

#include "surface_sampler.h"
#include "fresnel.h"

class BxDF
{
public:

    enum class BxDFType {
        DIFFUSE,
        DIELECTRIC,
        CONDUCTOR
    };

    using BxDFUniquePtr = std::unique_ptr< BxDF >;

    BxDF( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
          Fresnel::FresnelUniquePtr fresnel,
          BxDF::BxDFType bxdf_type ) :
        surface_sampler_( std::move( surface_sampler ) ),
        fresnel_( std::move( fresnel ) ),
        bxdf_type_{ bxdf_type }
    {};

    virtual glm::vec3 fr( const glm::vec3 &w_i,
                           const glm::vec3 &w_r ) const = 0;

    virtual glm::vec3 getNewDirection( const glm::vec3 &w_i ) const = 0;

    SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler_;

    Fresnel::FresnelUniquePtr fresnel_;

    BxDF::BxDFType bxdf_type_;
};

#endif /* BXDF_H_ */
