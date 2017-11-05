#include  "lambertian.h"

Lambertian::Lambertian( const glm::vec3 &reflectance,
                        SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                        Fresnel::FresnelUniquePtr fresnel ) :
        BxDF( std::move( surface_sampler ),
              std::move( fresnel ),
              BxDF::BxDFType::DIFFUSE ),
        reflectance_{ reflectance }
{ }

glm::vec3 Lambertian::fr( const glm::vec3 &w_i,
                           const glm::vec3 &w_r ) const
{
    ( void ) w_i; // unused variable
    ( void ) w_r; // unused variable

    if ( w_r.y < 0.0f )
        return glm::vec3{ 0.0f, 0.0f, 0.0f };
    
    return reflectance_ / kPi;
}

glm::vec3 Lambertian::getNewDirection( const glm::vec3 &w_i ) const
{
    if ( w_i.y < 0 )
        return -surface_sampler_->getSample( w_i );
    else
        return surface_sampler_->getSample( w_i );
}

