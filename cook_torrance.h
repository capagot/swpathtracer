#ifndef COOK_TORRANCE_H_
#define COOK_TORRANCE_H_

#include "onb.h"
#include "random.h"
#include "brdf.h"

class CookTorrance : public BRDF
{
public:

    CookTorrance( void );

    CookTorrance( const std::string &material_type, // TODO: change for enum !
                  double s,
                  double d,
                  double m,
                  double ior,
                  double k,
                  const glm::dvec3 &kd,
                  const glm::dvec3 &ks );

    glm::dvec3 fr( const glm::dvec3 &w_i,
                   const glm::dvec3 &w_r ) const
    {
        glm::dvec3 h = glm::normalize( w_i + w_r );
        double nh  = std::abs( h.y );
        double nv  = std::abs( w_i.y );
        double nl  = std::abs( w_r.y );
        double vh  = std::abs( glm::dot( w_i, h ) );
        double lh  = std::abs( glm::dot( w_r, h ) );

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
        glm::dvec3 f = ks_ + ( 1.0 - ks_ ) * pow( 1.0 - lh, 5.0 );

        glm::dvec3 specular = ( f * d * g ) / ( 4.0 * nv * nl );

        //return 2.0 * M_PI * ( ( d_ * kd_ ) / M_PI + ( s_ * specular ) ) * w_r.y ;

        return 2.0 * M_PI * s_ * specular * w_r.y ;

        //return  ( 1.0 / d ) * s_ * specular * w_r.y ;



    }

    glm::dvec3 getNewDirection( const glm::dvec3 &w_i,
                                const glm::dvec3 &normal,
                                RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) const;

private:

    std::string material_type_;

    double s_;

    double d_;

    double m_;

    double ior_;  // index of refraction

    double k_;    // extinction coefficient

    glm::dvec3 kd_;

    glm::dvec3 ks_;
};

#endif /* COOK_TORRANCE_H_ */
