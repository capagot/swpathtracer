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

