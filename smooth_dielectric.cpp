#include  "smooth_dielectric.h"

SmoothDielectric::SmoothDielectric( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                                    Fresnel::FresnelUniquePtr fresnel ) :
        BxDF( std::move( surface_sampler ),
              std::move( fresnel ),
              BxDF::BxDFType::DIELECTRIC )
{}

glm::dvec3 SmoothDielectric::fr( const glm::dvec3 &local_w_i,
                                 const glm::dvec3 &local_w_r ) const
{
    ( void ) local_w_i;

    return glm::dvec3{ 1.0 } / local_w_r.y;
}

glm::dvec3 SmoothDielectric::getNewDirection( const glm::dvec3 &local_w_i ) const
{
    return surface_sampler_->getSample( local_w_i );
}
