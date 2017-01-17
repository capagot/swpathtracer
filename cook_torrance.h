#ifndef COOK_TORRANCE_H_
#define COOK_TORRANCE_H_

#include "onb.h"
#include "random.h"
#include "brdf.h"

/* The CookTorrance class implements a microfacet-based BRDF model for rough specular surfaces.
 * It implements the rough specular term proposed originally in [1] and includes the normalizing
 * factors proposed in [2]. The Fresnel term currently used is based on a simple (and efficient)
 * approximation proposed originally in [1]. This will be replaced by a more accurate Fresnel term
 * in a future release.
 *
 *     [1] "A Reflectance Model for Computer Graphics"
 *         Cook, R. L. and Torrance, K. E.
 *         ACM Transactions on Graphics, ACM, 1982, 1, 7-24 
 *
 *     [2] "Microfacet Models for Refraction Through Rough Surfaces"
 *         Walter, B.; Marschner, S. R.; Li, H. and Torrance, K. E.
 *         Proceedings of the 18th Eurographics Conference on Rendering Techniques, Eurographics Association, 2007, 195-206.
 */
class CookTorrance : public BRDF
{
public:

    //CookTorrance( void );

    CookTorrance( double m,
                  double ior,
                  double k,
                  const glm::dvec3 &ks,
                  SurfaceSampler::SurfaceSamplerUniquePtr surface_sampler );

    glm::dvec3 fr( const glm::dvec3 &w_i,
                   const glm::dvec3 &w_r ) const
    {
        glm::dvec3 h = glm::normalize( w_i + w_r );
        double nh = std::abs( h.y );
        double nv = std::abs( w_i.y );
        double nl = std::abs( w_r.y );
        double vh = std::abs( glm::dot( w_i, h ) );
        double lh = std::abs( glm::dot( w_r, h ) );

        // Beckman normal distribution function
        double nh_2 = nh * nh;
        double m_2 = m_ * m_;
        double d1 = 1.0 / ( M_PI * m_2 * pow( nh, 4.0 ) );
        double d2 = ( nh_2 - 1.0 ) / ( m_2 * nh_2 );
        double d = d1 * exp( d2 );

        // Cook-Torrance geometric attenuation
        double g1 = 2.0 * nh / vh;
        double g = std::min( 1.0, std::min( g1 * nv, g1 * nl ) );

        // Fresnel term according to the Cook-Torrance approximation
        // TODO: Replace this Fresnel approximation by another one, more accurate. 
        glm::dvec3 f = ks_ + ( 1.0 - ks_ ) * pow( 1.0 - lh, 5.0 );

        glm::dvec3 specular = ( f * d * g ) / ( 4.0 * nv * nl );

        return ( 1.0 / surface_sampler_->getProbability( w_i, w_r ) ) * specular * w_r.y ;
    }

    glm::dvec3 getNewDirection( const glm::dvec3 &w_i,
                                const glm::dvec3 &normal,
                                RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) const;

private:

    double m_;          // roughness

    double ior_;        // index of refraction

    double k_;          // extinction coefficient

    glm::dvec3 ks_;     // specular color
};

#endif /* COOK_TORRANCE_H_ */

