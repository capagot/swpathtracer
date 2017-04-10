#ifndef FRESNEL_H_
#define FRESNEL_H_

#include <memory>

class Fresnel
{
public:

    typedef std::unique_ptr< Fresnel > FresnelUniquePtr;

    Fresnel( void )
    {}

    virtual glm::dvec3 value( const double theta ) const = 0;

};

class FresnelNoOp : public Fresnel
{
public:

    FresnelNoOp( void )
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

    // An Schlick's fresnel reflectance object can instanced
    // in two different ways:
    //
    //  1) Through the reflectance at normal incidence.
    //  2) Through two IOR's: one for the external and one for the internal material.

    FresnelSchlick( double eta1,
                    double eta2 ) :
        eta1_{ eta1 },
        eta2_{ eta2 }
    {
        glm::dvec3 a{ ( eta1 - eta2_ ) / ( eta1 + eta2_ ) };
        r0_ = a * a;
    }

    FresnelSchlick( const glm::dvec3 &reflectance_normal_incidence ) :
        r0_{ reflectance_normal_incidence }
    {}

    glm::dvec3 value( const double cos_theta ) const
    {
        return r0_ + ( 1.0 - r0_ ) * pow( 1.0 - cos_theta, 5.0 );
    }

private:

    double eta1_;

    double eta2_;

    glm::dvec3 r0_;

};

// TODO: Fresnel dielectric
//class FresnelDielectric : public Fresnel
//{
//}

class FresnelConductor : public Fresnel
{
public:

    FresnelConductor( const glm::dvec3 &eta,
                      const glm::dvec3 &kappa ) :
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
