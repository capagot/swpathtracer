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

    // Builds the tangencial reference frame from the input normal vector.
    // The V (up) axis of the new reference frame will be aligned to the input normal.
    onb.setUpFromV( normal ); 

    // Generates a random point, with uniform distribution, over the hemisphere 
    // in tangencial reference space.
    float r1 = rng();
    float r2 = rng();
    float phi = 2.0f * M_PI * r2;
    float sqrt_sin_theta = sqrt( 1.0f - r1 * r1 );

    glm::vec3 dir = { cos( phi ) * sqrt_sin_theta,
                      sin( phi ) * sqrt_sin_theta,
                      r1 };

    // Transform the point from the tangencial space to the world space.
    dir = onb.m_ * dir;        

    return dir;
}

