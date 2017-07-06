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

    virtual glm::dvec3 value( const double theta ) const = 0;

    FresnelType type_;
};

class FresnelNoOp : public Fresnel
{
public:

    FresnelNoOp( void ) :
        Fresnel{ Fresnel::FresnelType::NO_OP }
    {}

    glm::dvec3 value( const double cos_theta ) const
    {
        ( void ) cos_theta;

        return glm::dvec3{ 1.0, 1.0, 1.0 };
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

    FresnelSchlick( double eta_i,
                    double eta_t ) :
        Fresnel{ Fresnel::FresnelType::SCHLICK },
        schlick_type_{ FresnelSchlick::SchlickType::IOR },
        eta_i_{ eta_i },
        eta_t_{ eta_t }
    {}

    FresnelSchlick( const glm::dvec3 &reflectance_normal_incidence ) :
        Fresnel{ Fresnel::FresnelType::SCHLICK },
        schlick_type_{ FresnelSchlick::SchlickType::REFLECTANCE_AT_NORMAL_INCIDENCE },
        r0_{ reflectance_normal_incidence }
    {}

    glm::dvec3 value( const double cos_theta_i ) const
    {
        if ( schlick_type_ == FresnelSchlick::SchlickType::REFLECTANCE_AT_NORMAL_INCIDENCE )
            return r0_ + ( 1.0 - r0_ ) * pow( 1.0 - cos_theta_i, 5.0 );

        double eta_i;
        double eta_t;

        // schlick_type_ == FresnelSchlick::SchlickType::IOR
        if ( cos_theta_i > 0.0 ) // ray is entering into the surface
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
        double sin_theta_t = ( eta_i / eta_t ) * sqrt( 1.0 - cos_theta_i * cos_theta_i );

        if ( sin_theta_t >= 1.0 ) // TIR
            return glm::dvec3{ 1.0 };

        // The ray is refracted
        glm::dvec3 ratio_diff_eta{ ( eta_t - eta_i ) / ( eta_t + eta_i ) };
        glm::dvec3 r0{ ratio_diff_eta * ratio_diff_eta };

        double cos_theta_t = sqrt( 1.0 - sin_theta_t * sin_theta_t );

        return r0_ + ( 1.0 - r0_ ) * pow( 1.0 - cos_theta_t, 5.0 );
    }

private:

    SchlickType schlick_type_;

    double eta_i_;

    double eta_t_;

    glm::dvec3 r0_;

};

// TODO: Fresnel dielectric
//class FresnelDielectric : public Fresnel
//{
//}

//TODO: It's not used actually...
class FresnelConductor : public Fresnel
{
public:

    FresnelConductor( const glm::dvec3 &eta,
                      const glm::dvec3 &kappa ) :
        Fresnel{ Fresnel::FresnelType::COMPLEX },
        eta_{ eta },
        kappa_{ kappa }
    {}

    glm::dvec3 value( const double cos_theta ) const
    {
        glm::dvec3 eta_2_kappa_2 = ( eta_ * eta_ ) + ( kappa_ * kappa_ );
        double cos_theta_2 = cos_theta * cos_theta;
        glm::dvec3 a = eta_2_kappa_2 * cos_theta_2;
        glm::dvec3 b = 2.0 * eta_ * cos_theta;
        glm::dvec3 r2_parallel = ( a - b + 1.0 ) / ( a + b + 1.0 );
        glm::dvec3 r2_ortho = ( eta_2_kappa_2 - b + cos_theta_2 ) / ( eta_2_kappa_2 + b + cos_theta_2 );
        return glm::clamp( ( r2_parallel + r2_ortho ) * 0.5, glm::dvec3( 0.0, 0.0, 0.0 ), glm::dvec3( 1.0, 1.0, 1.0 ) );
    }

    glm::dvec3 eta_;

    glm::dvec3 kappa_;

};

#endif /* FRESNEL_H_ */
