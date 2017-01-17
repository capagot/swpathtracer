#include  "lambertian.h"

Lambertian::Lambertian( const glm::dvec3 &radiance,
                        SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler ) :
    BRDF( std::move( surface_sampler ) ),
    radiance_{ radiance }
{ }

glm::dvec3 Lambertian::getNewDirection( const glm::dvec3 &w_i,
                                        const glm::dvec3 &normal,
                                        RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) const
{
    ONB onb;

    // Builds a local tangential reference frame oriented according to the input 'normal' vector.
    // (the 'v' axis (up) of the tangential space will be aligned to the 'normal' input vector).
    onb.setFromV( normal );

    return onb.getBasisMatrix() * surface_sampler_->getSample( w_i );
}
