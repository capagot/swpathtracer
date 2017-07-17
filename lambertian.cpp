#include  "lambertian.h"

Lambertian::Lambertian( const glm::dvec3 &reflectance,
                        SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                        Fresnel::FresnelUniquePtr fresnel ) :
        BxDF( std::move( surface_sampler ),
              std::move( fresnel ),
              BxDF::BxDFType::DIFFUSE ),
        reflectance_{ reflectance }
{ }

glm::dvec3 Lambertian::fr( const glm::dvec3 &w_i,
                           const glm::dvec3 &w_r ) const
{
    ( void ) w_i; // unused variable
    ( void ) w_r; // unused variable

    if ( w_r.y < 0.0 )
        return glm::dvec3{ 0.0, 0.0, 0.0 };
    
    return reflectance_ / M_PI;
}

glm::dvec3 Lambertian::getNewDirection( const glm::dvec3 &w_i ) const
{
    if ( w_i.y < 0 )
        return -surface_sampler_->getSample( w_i );
    else
        return surface_sampler_->getSample( w_i );
}

