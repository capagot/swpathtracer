#include  "smooth_specular_reflection.h"

SmoothSpecularReflection::SmoothSpecularReflection( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                                                    Fresnel::FresnelUniquePtr fresnel ) :
        BxDF( std::move( surface_sampler ) ),
        fresnel_( std::move( fresnel ) )
{}

glm::dvec3 SmoothSpecularReflection::fr( const glm::dvec3 &w_i,
                                         const glm::dvec3 &w_r ) const
{
    ( void ) w_i;
    ( void ) w_r;

    return ( 1.0 / surface_sampler_->getProbability( w_i, w_r ) ) * fresnel_->value( 1.0 );
}

glm::dvec3 SmoothSpecularReflection::getNewDirection( const glm::dvec3 &w_i ) const
{
    return glm::dvec3{ -w_i.x, w_i.y, -w_i.z };
}
