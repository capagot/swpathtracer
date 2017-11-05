#ifndef SURFACE_SAMPLER_H_
#define SURFACE_SAMPLER_H_

#include <glm/glm.hpp>

#include "random.h"
#include "utils.h"

class SurfaceSampler
{
public:

    using SurfaceSamplerUniquePtr = std::unique_ptr< SurfaceSampler >;

    SurfaceSampler( void )
    {};

    virtual glm::vec3 getSample( const glm::vec3 &w_i ) = 0;

    virtual float getProbability( const glm::vec3 &w_i,
                                   const glm::vec3 &w_r ) = 0;

};

class SurfaceSamplerUniform : public SurfaceSampler
{
public:

    SurfaceSamplerUniform( RNG< std::uniform_real_distribution, float, std::mt19937 > &rng ) :
        rng_( rng )
    {};

    glm::vec3 getSample( const glm::vec3 &w_i )
    {
        ( void ) w_i;

        float r1 = rng_();
        float r2 = rng_();
        float phi = 2.0f * kPi * r2;
        float sqrtf_sin_theta = sqrtf( 1.0f - r1 * r1 );

        return glm::vec3{ cos( phi ) * sqrtf_sin_theta,
                           r1,
                           sin( phi ) * sqrtf_sin_theta };
    }

    float getProbability( const glm::vec3 &w_i,
                           const glm::vec3 &w_r )
    {
        ( void ) w_i; // unused variable
        ( void ) w_r; // unused variable

        return 1.0f / ( 2.0f * kPi );
    }

private:

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;

};

class SurfaceSamplerCosine : public SurfaceSampler
{
public:

    SurfaceSamplerCosine( RNG< std::uniform_real_distribution, float, std::mt19937 > &rng ) :
        rng_( rng )
    {};

    glm::vec3 getSample( const glm::vec3 &w_i )
    {
        ( void ) w_i; // unused variable

        float r1 = rng_();
        float r2 = rng_();
        float phi = 2.0f * kPi * r2;
        float cos_theta = sqrtf( r1 );
        float sqrtf_sin_theta = sqrtf( 1.0f - cos_theta * cos_theta );

        return glm::vec3{ cos( phi ) * sqrtf_sin_theta,
                           cos_theta,
                           sin( phi ) * sqrtf_sin_theta };
    }

    float getProbability( const glm::vec3 &w_i,
                           const glm::vec3 &w_r )
    {
        ( void ) w_i; // unused variable

        return w_r.y /  kPi;
    }

private:

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;

};

class SurfaceSamplerCookTorrance : public SurfaceSampler
{
public:

    SurfaceSamplerCookTorrance( RNG< std::uniform_real_distribution, float, std::mt19937 > &rng, float m ) :
        rng_( rng ),
        m_( m )
    {};

    glm::vec3 getSample( const glm::vec3 &w_i )
    {
        float r1 = rng_();
        float r2 = rng_();
        float phi = 2.0f * kPi * r2;
        float theta = atan( sqrtf( -( m_ * m_ ) * log( 1.0f - r1 ) ) );

        // computes the microfacet normal
        glm::vec3 m = { cos( phi ) * sin( theta ),
                         cos( theta ),
                         sin( phi ) * sin( theta ) };

        return 2.0f * m * glm::dot( m, w_i ) - w_i; // reflect w_i about m (the microfacet normal))
    }

    float getProbability( const glm::vec3 &w_i,
                           const glm::vec3 &w_r )
    {
        glm::vec3 h = glm::normalize( w_i + w_r );
        float nh = std::abs( h.y );
        float nh_2 = nh * nh;
        float m_2 = m_ * m_;
        float d1 = 1.0f / ( kPi * m_2 * nh * nh * nh * nh );
        float d2 = ( nh_2 - 1.0f ) / ( m_2 * nh_2 );

        float om = glm::dot( w_r, h );

        return ( d1 * exp( d2 ) * nh ) / ( 4.0f * ( om ) );
    }

private:

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;

    float m_;
};

class SurfaceSamplerSmoothConductor : public SurfaceSampler
{
public:

    SurfaceSamplerSmoothConductor( void )
    {};

    glm::vec3 getSample( const glm::vec3 &w_i )
    {
        return glm::vec3{ -w_i.x, w_i.y, -w_i.z };
    }

    float getProbability( const glm::vec3 &w_i,
                           const glm::vec3 &w_r )
    {
        ( void ) w_i;
        ( void ) w_r;

        return 1.0f;
    }

};

class SurfaceSamplerSmoothRefraction : public SurfaceSampler
{
public:

    SurfaceSamplerSmoothRefraction( RNG< std::uniform_real_distribution, float, std::mt19937 > &rng,
                                    float eta_i,
                                    float eta_t) :
        rng_( rng ),
        eta_i_( eta_i ),
        eta_t_( eta_t )
    {};

    glm::vec3 getSample( const glm::vec3 &w_i )
    {

        glm::vec3 n{ 0.0f, 1.0f, 0.0f }; //  normal vector of the local space
        float eta_i;
        float eta_t;
        float signal;

        if ( w_i.y > 0.0f )  // w_i is entering the denser material
        {
            eta_i = eta_i_;
            eta_t = eta_t_;
            signal = 1.0f;
        }
        else
        {
            eta_i = eta_t_;
            eta_t = eta_i_;
            signal = -1.0f;
        }

        float a = 1.0f - ( eta_i * eta_i ) / ( eta_t * eta_t ) * ( 1.0f - w_i.y * w_i.y );

        if ( a < 0.0f )
            return glm::vec3{ -w_i.x, w_i.y, -w_i.z };

        glm::vec3 t = glm::normalize( ( eta_i / eta_t ) * ( n * w_i.y - w_i ) - signal * n * sqrtf( a ) );
        float cos_theta =  ( w_i.y > 0.0f ) ? w_i.y : t.y ;

        // Fresnel Schlick
        glm::vec3 ratio_diff_eta{ ( eta_t - eta_i ) / ( eta_t + eta_i ) };
        glm::vec3 r0{ ratio_diff_eta * ratio_diff_eta };
        float cos_th_5 = 1.0f - cos_theta;
        cos_th_5 = cos_th_5 * cos_th_5 * cos_th_5 * cos_th_5 * cos_th_5;
        //glm::vec3 fresnel = r0 + ( 1.0f - r0 ) * pow( 1.0f - cos_theta, 5.0f );
        glm::vec3 fresnel = r0 + ( 1.0f - r0 ) * cos_th_5;        

        // Fresnel dielectric
        //float cos_th_i = fabs( w_i.y );
        //float cos_th_t = fabs( t.y );
        //float r_parallel = ( eta_t * cos_th_i - eta_i * cos_th_t ) / ( eta_t * cos_th_i + eta_i * cos_th_t );
        //float r_ortho    = ( eta_i * cos_th_i - eta_t * cos_th_t ) / ( eta_i * cos_th_i + eta_t * cos_th_t );
        //glm::vec3 fresnel = glm::vec3{ ( r_parallel * r_parallel + r_ortho * r_ortho ) * 0.5f };

        float max_reflectance = std::max( std::max( fresnel[0], fresnel[1] ), fresnel[2] );

        if ( rng_() < max_reflectance )
            return glm::vec3{ -w_i.x, w_i.y, -w_i.z };
        else
            return t;
    }

    float getProbability( const glm::vec3 &w_i,
                           const glm::vec3 &w_r )
    {
        ( void ) w_i;
        ( void ) w_r;

        // We are using importance sampling, so we can use probability = 1.
        return 1.0f;
    }

private:

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;

    float eta_i_;

    float eta_t_;

};
//*/

#endif /* SURFACE_SAMPLER_H_ */
