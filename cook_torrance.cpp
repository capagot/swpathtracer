#include  "cook_torrance.h"

CookTorrance::CookTorrance( void )
{ }

CookTorrance::CookTorrance( const std::string &material_type, // TODO: change for enum !
                            double s,
                            double d,
                            double m,
                            double ior,
                            double k,
                            const glm::dvec3 &kd,
                            const glm::dvec3 &ks ) :
        material_type_{ material_type },
        s_{ s },
        d_{ d },
        m_{ m },
        ior_{ ior },
        k_{ k },
        kd_{ kd },
        ks_{ ks }
{ }

glm::dvec3 CookTorrance::getNewDirection( const glm::dvec3 &w_i,
                                          const glm::dvec3 &normal,
                                          RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) const
{
    //return glm::reflect( w_i, normal );

    ONB onb;

    // Builds a local tangential reference frame oriented according to the input 'normal' vector.
    // (the 'v' axis (up) of the tangential space will be aligned to the 'normal' input vector).
    onb.setFromV( normal );

    // Generates the unitary vector 'dir', which points from the origin of the local tangential frame
    // towards a random point uniformly distributed over the hemisphere centered at the origin.

    double r1 = rng();
    double r2 = rng();
    double phi = 2.0 * M_PI * r2;

    double sqrt_sin_theta = sqrt( 1.0 - r1 * r1 );
    glm::dvec3 dir = { cos( phi ) * sqrt_sin_theta,
                      r1,
                      sin( phi ) * sqrt_sin_theta };

    //double theta_m = atan( sqrt( -m_ * m_ * log( 1.0 - r1 ) ) );
    //glm::dvec3 dir = { cos( phi ) * sin( theta_m ),
    //                   r1,
    //                   sin( phi ) * sin( theta_m ) };

    // A failed tentative of importance sampling the cook torrance BRDF
    //float theta = atan( -pow( m_, 2.0 ) * log( 1.0 - r1 ) );
    //glm::dvec3 dir = { cos( phi ) * sin( theta ),
    //                   cos( theta ),
    //                   sin( phi ) * sin( theta ) };

    // Transforms the 'dir' vector from the local tangential space into the space
    // where the local space is defined.
    //return onb.m_ * dir;
    return onb.getBasisMatrix() * dir;
}

/*
glm::dvec3 CookTorrance::getNewDirection( const glm::dvec3 &normal,
                                          RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) const
{
        ONB onb;

    // Builds a local tangential reference frame oriented according to the input 'normal' vector.
    // (the 'v' axis (up) of the tangential space will be aligned to the 'normal' input vector).
    onb.setFromV( normal );

    // Generates the unitary vector 'dir', which points from the origin of the local tangential frame
    // towards a random point uniformly distributed over the hemisphere centered at the origin.
    double r1 = rng();
    double r2 = rng();
    double phi = 2.0 * M_PI * r2;
    double sqrt_sin_theta = sqrt( 1.0 - r1 * r1 );

    glm::dvec3 dir = { cos( phi ) * sqrt_sin_theta,
                      r1,
                      sin( phi ) * sqrt_sin_theta };

    // Transforms the 'dir' vector from the local tangential space into the space
    // where the local space is defined.
    //return onb.m_ * dir;
    return onb.getBasisMatrix() * dir;
}
//*/
