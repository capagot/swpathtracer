/*
 * spectrum.cpp
 *
 *  Created on: May 14, 2016
 *      Author: christian
 */

#include "spectrum.h"
#include "onb.h"

Spectrum::Spectrum( void )
{ }

Spectrum::Spectrum( const glm::vec3 &spectrum ) :
        spectrum_{ spectrum }
{ }
    
glm::vec3 Spectrum::getDirection( const glm::vec3 &normal,
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
