#include  "lambertian.h"

Lambertian::Lambertian( void )
{ }

Lambertian::Lambertian( const glm::dvec3 &radiance ) :
    radiance_{ radiance }
{ }

glm::dvec3 Lambertian::getNewDirection( const glm::dvec3 &normal,
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

