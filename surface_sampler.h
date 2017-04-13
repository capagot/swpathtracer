#ifndef SURFACE_SAMPLER_H_
#define SURFACE_SAMPLER_H_

#include <glm/glm.hpp>

#include "random.h"

class SurfaceSampler
{
public:

    typedef std::unique_ptr< SurfaceSampler > SurfaceSamplerUniquePtr;

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

class SurfaceSamplerSmoothSpecularReflection : public SurfaceSampler
{
public:

    SurfaceSamplerSmoothSpecularReflection( void )
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

        return w_r.y / 1.0;
    }

};

#endif /* SURFACE_SAMPLER_H_ */
