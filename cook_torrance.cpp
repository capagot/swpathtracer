#include  "cook_torrance.h"

CookTorrance::CookTorrance( double m,
                            SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                            Fresnel::FresnelUniquePtr fresnel ) :
        BxDF( std::move( surface_sampler ),
              std::move( fresnel ),
              BxDF::BxDFType::CONDUCTOR ),
        m_{ m }
{}

glm::dvec3 CookTorrance::fr( const glm::dvec3 &w_i,
                             const glm::dvec3 &w_r ) const
{
    // Since the microfacet normal may not be aligned with the average surface normal,
    // it can be the case that the generated w_r might point into the average surface 
    // (i.e., in terms of a local coordinate system, it's 'y' coordinate may be negative). 
    // In this case, it is not possible to sample the Cook-Torrance BRDF because it is
    // defined only for hemisphere above the intersection point. Thus, in these cases, 
    // the BRDF simply return 0. 
    // -- That problem, and the adopted solution, are mentioned in the Eric Veach's
    // thesis, in the Section about shading normals.
    if ( w_r.y < 0.0 )
        return glm::dvec3{ 0.0, 0.0, 0.0 };

    glm::dvec3 h = glm::normalize( w_i + w_r );
    double nh = std::abs( h.y );
    double nv = std::abs( w_i.y );
    double nl = std::abs( w_r.y );
    double vh = std::abs( glm::dot( w_i, h ) );
    double lh = std::abs( glm::dot( w_r, h ) );

    // Beckmann normal distribution function
    double nh_2 = nh * nh;
    double m_2 = m_ * m_;
    double d1 = 1.0 / ( M_PI * m_2 * nh_2 * nh_2 );
    double d2 = ( nh_2 - 1.0 ) / ( m_2 * nh_2 );
    double d = d1 * exp( d2 );

    // Cook-Torrance geometric attenuation
    double g1 = 2.0 * nh / vh;
    double g = std::min( 1.0, std::min( g1 * nv, g1 * nl ) );

    // Fresnel term
    glm::dvec3 f = fresnel_->value( lh );

    glm::dvec3 rough_specular_term = ( f * d * g ) / ( 4.0 * nv * nl );

    return rough_specular_term;
}

glm::dvec3 CookTorrance::getNewDirection( const glm::dvec3 &w_i ) const
{
    return surface_sampler_->getSample( w_i );
}

