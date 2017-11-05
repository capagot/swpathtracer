#ifndef FRESNEL_H_
#define FRESNEL_H_

#include <memory>

class Fresnel
{
public:

    enum class FresnelType {
        NO_OP,
        SCHLICK,
        DIELECTRIC,
        COMPLEX,
    };

    using FresnelUniquePtr = std::unique_ptr< Fresnel >;

    Fresnel( Fresnel::FresnelType type ) :
        type_{ type }
    {}

    virtual glm::vec3 value( const float theta ) const = 0;

    FresnelType type_;
};

class FresnelNoOp : public Fresnel
{
public:

    FresnelNoOp( void ) :
        Fresnel{ Fresnel::FresnelType::NO_OP }
    {}

    glm::vec3 value( const float cos_theta ) const
    {
        ( void ) cos_theta;

        return glm::vec3{ 1.0f, 1.0f, 1.0f };
    }

};

class FresnelSchlick : public Fresnel
{
public:

    enum class SchlickType {
        REFLECTANCE_AT_NORMAL_INCIDENCE,
        IOR
    };

    // A Schlick's fresnel reflectance object can instanced
    // in two different ways:
    //
    //  1) Through the reflectance at normal incidence.
    //  2) Through two IOR's: one for the externtal and one for the internal material.

    FresnelSchlick( float eta_i,
                    float eta_t ) :
        Fresnel{ Fresnel::FresnelType::SCHLICK },
        schlick_type_{ FresnelSchlick::SchlickType::IOR },
        eta_i_{ eta_i },
        eta_t_{ eta_t }
    {}

    FresnelSchlick( const glm::vec3 &reflectance_normal_incidence ) :
        Fresnel{ Fresnel::FresnelType::SCHLICK },
        schlick_type_{ FresnelSchlick::SchlickType::REFLECTANCE_AT_NORMAL_INCIDENCE },
        r0_{ reflectance_normal_incidence }
    {}

    glm::vec3 value( const float cos_theta_i ) const
    {
        if ( schlick_type_ == FresnelSchlick::SchlickType::REFLECTANCE_AT_NORMAL_INCIDENCE )
        {
            float cos_th_i_5 = 1.0f - cos_theta_i;
            cos_th_i_5 = cos_th_i_5 * cos_th_i_5 * cos_th_i_5 * cos_th_i_5 * cos_th_i_5;  

            //return r0_ + ( 1.0f - r0_ ) * pow( 1.0f - cos_theta_i, 5.0f );
            return r0_ + ( 1.0f - r0_ ) * cos_th_i_5;
        }

        float eta_i;
        float eta_t;

        // schlick_type_ == FresnelSchlick::SchlickType::IOR
        if ( cos_theta_i > 0.0f ) // ray is entering into the surface
        {
            eta_i = eta_i_;
            eta_t = eta_t_;
        }
        else // ray is exiting the surface
        {
            eta_i = eta_t_;
            eta_t = eta_i_;
        }

        // From the Snell's law
        float sin_theta_t = ( eta_i / eta_t ) * sqrtf( 1.0f - cos_theta_i * cos_theta_i );

        if ( sin_theta_t >= 1.0f ) // TIR
            return glm::vec3{ 1.0f };

        // The ray is refracted
        glm::vec3 ratio_diff_eta{ ( eta_t - eta_i ) / ( eta_t + eta_i ) };
        glm::vec3 r0{ ratio_diff_eta * ratio_diff_eta };

        float cos_theta_t = sqrtf( 1.0f - sin_theta_t * sin_theta_t );

        //return r0_ + ( 1.0f - r0_ ) * pow( 1.0f - cos_theta_t, 5.0f );
        float cos_th_t_5 = 1.0f - cos_theta_t;
        cos_th_t_5 = cos_th_t_5 * cos_th_t_5 * cos_th_t_5 * cos_th_t_5 * cos_th_t_5;  
        return r0_ + ( 1.0f - r0_ ) * cos_th_t_5;

    }

private:

    SchlickType schlick_type_;

    float eta_i_;

    float eta_t_;

    glm::vec3 r0_;

};

// TODO: Fresnel dielectric
//class FresnelDielectric : public Fresnel
//{
//}

//TODO: It's not used actually...
class FresnelConductor : public Fresnel
{
public:

    FresnelConductor( const glm::vec3 &eta,
                      const glm::vec3 &kappa ) :
        Fresnel{ Fresnel::FresnelType::COMPLEX },
        eta_{ eta },
        kappa_{ kappa }
    {}

    glm::vec3 value( const float cos_theta ) const
    {
        glm::vec3 eta_2_kappa_2 = ( eta_ * eta_ ) + ( kappa_ * kappa_ );
        float cos_theta_2 = cos_theta * cos_theta;
        glm::vec3 a = eta_2_kappa_2 * cos_theta_2;
        glm::vec3 b = 2.0f * eta_ * cos_theta;
        glm::vec3 r2_parallel = ( a - b + 1.0f ) / ( a + b + 1.0f );
        glm::vec3 r2_ortho = ( eta_2_kappa_2 - b + cos_theta_2 ) / ( eta_2_kappa_2 + b + cos_theta_2 );
        return glm::clamp( ( r2_parallel + r2_ortho ) * 0.5f, glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ) );
    }

    glm::vec3 eta_;

    glm::vec3 kappa_;

};

#endif /* FRESNEL_H_ */
