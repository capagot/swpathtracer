#include  "cook_torrance.h"

CookTorrance::CookTorrance( float m,
                            SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler,
                            Fresnel::FresnelUniquePtr fresnel ) :
        BxDF( std::move( surface_sampler ),
              std::move( fresnel ),
              BxDF::BxDFType::CONDUCTOR ),
        m_{ m }
{}

glm::vec3 CookTorrance::fr( const glm::vec3 &w_i,
                             const glm::vec3 &w_r ) const
{
    // Since the microfacet normal may not be aligned with the average surface normal,
    // it can be the case that the generated w_r might point into the average surface 
    // (i.e., in terms of a local coordinate system, it's 'y' coordinate may be negative). 
    // In this case, it is not possible to sample the Cook-Torrance BRDF because it is
    // defined only for hemisphere above the intersection point. Thus, in these cases, 
    // the BRDF simply return 0. 
    // -- That problem, and the adopted solution, are mentioned in the Eric Veach's
    // thesis, in the Section about shading normals.
    if ( w_r.y < 0.0f )
        return glm::vec3{ 0.0f, 0.0f, 0.0f };

    glm::vec3 h = glm::normalize( w_i + w_r );
    float nh = std::abs( h.y );
    float nv = std::abs( w_i.y );
    float nl = std::abs( w_r.y );

    if ( ( nv == 0.0f ) || ( nl == 0.0f ) )
    {
        std::clog << "Cook Torrance nv/nl == 0.0f!\n";
        return glm::vec3{ 0.0f };
    }
    
    float vh = std::abs( glm::dot( w_i, h ) );
    float lh = std::abs( glm::dot( w_r, h ) );

    // Beckmann normal distribution function
    float nh_2 = nh * nh;
    float m_2 = m_ * m_;
    float d1 = 1.0f / ( kPi * m_2 * nh_2 * nh_2 );
    float d2 = ( nh_2 - 1.0f ) / ( m_2 * nh_2 );
    float d = d1 * exp( d2 );

    // Cook-Torrance geometric attenuation
    float g1 = 2.0f * nh / vh;
    float g = std::min( 1.0f, std::min( g1 * nv, g1 * nl ) );

    // Fresnel term
    glm::vec3 f = fresnel_->value( lh );

    glm::vec3 rough_specular_term = ( f * d * g ) / ( 4.0f * nv * nl );

    return rough_specular_term;
}

glm::vec3 CookTorrance::getNewDirection( const glm::vec3 &w_i ) const
{
    return surface_sampler_->getSample( w_i );
}

