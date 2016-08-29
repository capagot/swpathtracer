/*
 * spectrum.cpp
 *
 *  Created on: May 14, 2016
 *      Author: christian
 */

#include "spectrum.h"

Spectrum::Spectrum( void )
{ }

Spectrum::Spectrum( const glm::vec3 &spectrum ) :
        spectrum_{ spectrum }
{ }
    
glm::vec3 Spectrum::getDirection( const glm::vec3 &normal )
{
    return glm::vec3( 0.0f, 0.0f, 1.0f );
}

