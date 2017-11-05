#include  "smooth_conductor.h"

SmoothConductor::SmoothConductor( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                                  Fresnel::FresnelUniquePtr fresnel ) :
        BxDF( std::move( surface_sampler ),
              std::move( fresnel ),
              BxDF::BxDFType::CONDUCTOR )
{}

glm::vec3 SmoothConductor::fr( const glm::vec3 &w_i,
                                const glm::vec3 &w_r ) const
{
    ( void ) w_i; // unused variable

    if ( w_r.y == 0.0f )
        return glm::vec3{ 0.0f };

    return fresnel_->value( w_r.y ) / w_r.y;
}

glm::vec3 SmoothConductor::getNewDirection( const glm::vec3 &w_i ) const
{
    return surface_sampler_->getSample( w_i );
}

