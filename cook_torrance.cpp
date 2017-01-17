#include  "cook_torrance.h"

//CookTorrance::CookTorrance( void )
//{}

CookTorrance::CookTorrance( double m,
                            double ior,
                            double k,
                            const glm::dvec3 &ks,
                            SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler ) :
        BRDF( std::move( surface_sampler ) ),
        m_{ m },
        ior_{ ior },
        k_{ k },
        ks_{ ks }
{}

glm::dvec3 CookTorrance::getNewDirection( const glm::dvec3 &w_i,
                                          const glm::dvec3 &normal,
                                          RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) const
{
    ONB onb;

    // Builds a local tangential reference frame oriented according to the input 'normal' vector.
    // (the 'v' axis (up) of the tangential space will be aligned to the 'normal' input vector).
    onb.setFromV( normal );

    return onb.getBasisMatrix() * surface_sampler_->getSample( w_i );
}

