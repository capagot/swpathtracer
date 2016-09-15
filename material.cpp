/*
 * material.cpp
 *
 *  Created on: Jun 5, 2016
 *      Author: christian
 */

#include  "material.h"

Lambertian::Lambertian( void ) :
        brdf_{ glm::vec3{ 0.8f, 0.8f, 0.8f } },
        emitted_{ glm::vec3{ 0.0f, 0.0f, 0.0f } }
{ }

Lambertian::Lambertian( const glm::vec3 &brdf, const glm::vec3 &emitted ) :
        brdf_{ brdf },
        emitted_{ emitted }
{ }

glm::vec3 Lambertian::getNewDirection( const glm::vec3 &normal,
                                       RNG< std::uniform_real_distribution, float, std::mt19937 > &rng )
{
    ONB onb;

    // Builds a local tangential reference frame oriented according to the input 'normal' vector.
    // (the 'v' axis (up) of the tangential space will be aligned to the 'normal' input vector).
    onb.setFromV( normal );

    // Generates the unitary vector 'dir', which points from the origin of the local tangential frame
    // towards a random point uniformly distributed over the hemisphere centered at the origin.
    float r1 = rng();
    float r2 = rng();
    float phi = 2.0f * M_PI * r2;
    float sqrt_sin_theta = sqrt( 1.0f - r1 * r1 );

    glm::vec3 dir = { cos( phi ) * sqrt_sin_theta,
                      r1,
                      sin( phi ) * sqrt_sin_theta };

    // Transforms the 'dir' vector from the local tangential space into the space
    // where the local space is defined.
    return onb.m_ * dir;
}
