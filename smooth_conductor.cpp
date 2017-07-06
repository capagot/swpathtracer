#include  "smooth_conductor.h"

SmoothConductor::SmoothConductor( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                                  Fresnel::FresnelUniquePtr fresnel ) :
        BxDF( std::move( surface_sampler ),
              std::move( fresnel ),
              BxDF::BxDFType::CONDUCTOR )
{}

glm::dvec3 SmoothConductor::fr( const glm::dvec3 &w_i,
                                const glm::dvec3 &w_r ) const
{
    ( void ) w_i; // unused variable

    return fresnel_->value( w_r.y ) / w_r.y;
}

glm::dvec3 SmoothConductor::getNewDirection( const glm::dvec3 &w_i ) const
{
    return surface_sampler_->getSample( w_i );
}

