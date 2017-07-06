#ifndef SURFACE_SAMPLER_H_
#define SURFACE_SAMPLER_H_

#include <glm/glm.hpp>

#include "random.h"

class SurfaceSampler
{
public:

    using SurfaceSamplerUniquePtr = std::unique_ptr< SurfaceSampler >;

    SurfaceSampler( void )
    {};

    virtual glm::dvec3 getSample( const glm::dvec3 &w_i ) = 0;

    virtual double getProbability( const glm::dvec3 &w_i,
                                   const glm::dvec3 &w_r ) = 0;

};

class SurfaceSamplerUniform : public SurfaceSampler
{
public:

    SurfaceSamplerUniform( RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) :
        rng_( rng )
    {};

    glm::dvec3 getSample( const glm::dvec3 &w_i )
    {
        ( void ) w_i;

        double r1 = rng_();
        double r2 = rng_();
        double phi = 2.0 * M_PI * r2;
        double sqrt_sin_theta = sqrt( 1.0 - r1 * r1 );

        return glm::dvec3{ cos( phi ) * sqrt_sin_theta,
                           r1,
                           sin( phi ) * sqrt_sin_theta };
    }

    double getProbability( const glm::dvec3 &w_i,
                           const glm::dvec3 &w_r )
    {
        ( void ) w_i; // unused variable
        ( void ) w_r; // unused variable

        return 1.0 / ( 2.0 * M_PI );
    }

private:

    RNG< std::uniform_real_distribution, double, std::mt19937 > rng_;

};

class SurfaceSamplerCosine : public SurfaceSampler
{
public:

    SurfaceSamplerCosine( RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) :
        rng_( rng )
    {};

    glm::dvec3 getSample( const glm::dvec3 &w_i )
    {
        ( void ) w_i; // unused variable

        double r1 = rng_();
        double r2 = rng_();
        double phi = 2.0 * M_PI * r2;
        double cos_theta = sqrt( r1 );
        double sqrt_sin_theta = sqrt( 1.0 - cos_theta * cos_theta );

        return glm::dvec3{ cos( phi ) * sqrt_sin_theta,
                           cos_theta,
                           sin( phi ) * sqrt_sin_theta };
    }

    double getProbability( const glm::dvec3 &w_i,
                           const glm::dvec3 &w_r )
    {
        ( void ) w_i; // unused variable

        return w_r.y /  M_PI;
    }

private:

    RNG< std::uniform_real_distribution, double, std::mt19937 > rng_;

};

class SurfaceSamplerCookTorrance : public SurfaceSampler
{
public:

    SurfaceSamplerCookTorrance( RNG< std::uniform_real_distribution, double, std::mt19937 > &rng, double m ) :
        rng_( rng ),
        m_( m )
    {};

    glm::dvec3 getSample( const glm::dvec3 &w_i )
    {
        double r1 = rng_();
        double r2 = rng_();
        double phi = 2.0 * M_PI * r2;
        double theta = atan( sqrt( -( m_ * m_ ) * log( 1.0 - r1 ) ) );

        // computes the microfacet normal
        glm::dvec3 m = { cos( phi ) * sin( theta ),
                         cos( theta ),
                         sin( phi ) * sin( theta ) };

        return 2.0 * m * glm::dot( m, w_i ) - w_i; // reflect w_i about m (the microfacet normal))
    }

    double getProbability( const glm::dvec3 &w_i,
                           const glm::dvec3 &w_r )
    {
        glm::dvec3 h = glm::normalize( w_i + w_r );
        double nh = std::abs( h.y );
        double nh_2 = nh * nh;
        double m_2 = m_ * m_;
        double d1 = 1.0 / ( M_PI * m_2 * pow( nh, 4.0 ) );
        double d2 = ( nh_2 - 1.0 ) / ( m_2 * nh_2 );

        double om = glm::dot( w_r, h );

        return ( d1 * exp( d2 ) * nh ) / ( 4.0 * ( om ) );
    }

private:

    RNG< std::uniform_real_distribution, double, std::mt19937 > rng_;

    double m_;
};

class SurfaceSamplerSmoothConductor : public SurfaceSampler
{
public:

    SurfaceSamplerSmoothConductor( void )
    {};

    glm::dvec3 getSample( const glm::dvec3 &w_i )
    {
        return glm::dvec3{ -w_i.x, w_i.y, -w_i.z };
    }

    double getProbability( const glm::dvec3 &w_i,
                           const glm::dvec3 &w_r )
    {
        ( void ) w_i;
        ( void ) w_r;

        return 1.0;
    }

};

class SurfaceSamplerSmoothRefraction : public SurfaceSampler
{
public:

    SurfaceSamplerSmoothRefraction( RNG< std::uniform_real_distribution, double, std::mt19937 > &rng,
                                    double eta_i,
                                    double eta_t) :
        rng_( rng ),
        eta_i_( eta_i ),
        eta_t_( eta_t )
    {};

    glm::dvec3 getSample( const glm::dvec3 &w_i )
    {

        glm::dvec3 n{ 0.0, 1.0, 0.0 }; //  normal vector of the local space
        double eta_i;
        double eta_t;
        double signal;

        if ( w_i.y > 0.0 )  // w_i is entering the denser material
        {
            eta_i = eta_i_;
            eta_t = eta_t_;
            signal = 1.0;
        }
        else
        {
            eta_i = eta_t_;
            eta_t = eta_i_;
            signal = -1.0;
        }

        double a = 1.0 - ( eta_i * eta_i ) / ( eta_t * eta_t ) * ( 1.0 - w_i.y * w_i.y );

        if ( a < 0.0 )
            return glm::dvec3{ -w_i.x, w_i.y, -w_i.z };

        glm::dvec3 t = glm::normalize( ( eta_i / eta_t ) * ( n * w_i.y - w_i ) - signal * n * sqrt( a ) );
        double cos_theta =  ( w_i.y > 0.0 ) ? w_i.y : t.y ;

        // Fresnel Schlick
        glm::dvec3 ratio_diff_eta{ ( eta_t - eta_i ) / ( eta_t + eta_i ) };
        glm::dvec3 r0{ ratio_diff_eta * ratio_diff_eta };
        glm::dvec3 fresnel = r0 + ( 1.0 - r0 ) * pow( 1.0 - cos_theta, 5.0 );

        // Fresnel dielectric
        //double cos_th_i = fabs( w_i.y );
        //double cos_th_t = fabs( t.y );
        //double r_parallel = ( eta_t * cos_th_i - eta_i * cos_th_t ) / ( eta_t * cos_th_i + eta_i * cos_th_t );
        //double r_ortho    = ( eta_i * cos_th_i - eta_t * cos_th_t ) / ( eta_i * cos_th_i + eta_t * cos_th_t );
        //glm::dvec3 fresnel = glm::dvec3{ ( r_parallel * r_parallel + r_ortho * r_ortho ) * 0.5 };

        double max_reflectance = std::max( std::max( fresnel[0], fresnel[1] ), fresnel[2] );

        if ( rng_() < max_reflectance )
            return glm::dvec3{ -w_i.x, w_i.y, -w_i.z };
        else
            return t;
    }

    double getProbability( const glm::dvec3 &w_i,
                           const glm::dvec3 &w_r )
    {
        ( void ) w_i;
        ( void ) w_r;

        // We are using importance sampling, so we can use probability = 1.
        return 1.0;
    }

private:

    RNG< std::uniform_real_distribution, double, std::mt19937 > rng_;

    double eta_i_;

    double eta_t_;

};
//*/

#endif /* SURFACE_SAMPLER_H_ */
