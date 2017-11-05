#include  "smooth_dielectric.h"

SmoothDielectric::SmoothDielectric( SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                                    Fresnel::FresnelUniquePtr fresnel ) :
        BxDF( std::move( surface_sampler ),
              std::move( fresnel ),
              BxDF::BxDFType::DIELECTRIC )
{}

glm::vec3 SmoothDielectric::fr( const glm::vec3 &local_w_i,
                                 const glm::vec3 &local_w_r ) const
{
    ( void ) local_w_i;

    if ( local_w_r.y == 0.0 )
    {
        return glm::vec3{ 0.0f };
    }   

    return glm::vec3{ 1.0f } / local_w_r.y;
}

glm::vec3 SmoothDielectric::getNewDirection( const glm::vec3 &local_w_i ) const
{
    return surface_sampler_->getSample( local_w_i );
}
