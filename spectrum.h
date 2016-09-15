/*
 * spectrum.h
 *
 *  Created on: May 14, 2016
 *      Author: christian
 */

#ifndef SPECTRUM_H_
#define SPECTRUM_H_

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "random.h"

class Spectrum
{
public:

    Spectrum( void );

    Spectrum( const glm::vec3 &spectrum );

    void print( void )
    {
        std::clog << "spectrum_: [" << spectrum_[0] << ", " << spectrum_[1] << ", " << spectrum_[2] << "]";
    }

    glm::vec3 spectrum_ = { 0.0f, 0.0f, 0.0f };

};

#endif /* SPECTRUM_H_ */

