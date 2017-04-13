#include  "lambertian.h"

Lambertian::Lambertian( const glm::dvec3 &radiance,
                        SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler ) :
        BxDF( std::move( surface_sampler ) ),
        radiance_{ radiance }
{ }

glm::dvec3 Lambertian::fr( const glm::dvec3 &w_i,
                           const glm::dvec3 &w_r ) const
{
    ( void ) w_i; // unused variable

    //return ( 1.0 / surface_sampler_->getProbability( w_i, w_r ) ) * ( radiance_ / M_PI ) *  w_r.y;
    return ( 1.0 / surface_sampler_->getProbability( w_i, w_r ) ) * ( radiance_ / M_PI );

}

glm::dvec3 Lambertian::getNewDirection( const glm::dvec3 &w_i ) const
{
    return surface_sampler_->getSample( w_i );
}
