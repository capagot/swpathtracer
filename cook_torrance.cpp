#include  "cook_torrance.h"

CookTorrance::CookTorrance( double m,
                            SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                            Fresnel::FresnelUniquePtr fresnel ) :
        BxDF( std::move( surface_sampler ) ),
        m_{ m },
        fresnel_( std::move( fresnel ) )
{}

glm::dvec3 CookTorrance::fr( const glm::dvec3 &w_i,
                             const glm::dvec3 &w_r ) const
{
    glm::dvec3 h = glm::normalize( w_i + w_r );
    double nh = std::abs( h.y );
    double nv = std::abs( w_i.y );
    double nl = std::abs( w_r.y );
    double vh = std::abs( glm::dot( w_i, h ) );
    double lh = std::abs( glm::dot( w_r, h ) );

    // Beckmann normal distribution function
    double nh_2 = nh * nh;
    double m_2 = m_ * m_;
    double d1 = 1.0 / ( M_PI * m_2 * pow( nh, 4.0 ) );
    double d2 = ( nh_2 - 1.0 ) / ( m_2 * nh_2 );
    double d = d1 * exp( d2 );

    // Cook-Torrance geometric attenuation
    double g1 = 2.0 * nh / vh;
    double g = std::min( 1.0, std::min( g1 * nv, g1 * nl ) );

    // Fresnel term
    glm::dvec3 f = fresnel_->value( lh );

    glm::dvec3 rough_specular_term = ( f * d * g ) / ( 4.0 * nv * nl );

    return ( 1.0 / surface_sampler_->getProbability( w_i, w_r ) ) * rough_specular_term * w_r.y ;
}

glm::dvec3 CookTorrance::getNewDirection( const glm::dvec3 &w_i ) const
{
    return surface_sampler_->getSample( w_i );
}

