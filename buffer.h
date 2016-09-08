/*
 * buffer.h
 *
 *  Created on: Apr 22, 2016
 *      Author: christian
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <iostream>
#include <fstream>
#include <vector>
#include "spectrum.h"

class Buffer
{
public:

    Buffer( unsigned int h_resolution,
            unsigned int v_resolution,
            int spectrum_num_samples );

    ~Buffer( void );

    void printInfo( void ) const;

    void save( const std::string &filename ) const;

    unsigned int h_resolution_ = 512;

    unsigned int v_resolution_ = 512;

    int spectrum_num_samples_ = 3;

    std::vector< std::vector< Spectrum > > buffer_data_;

private:



    float clamp( float x ) const
    {
        return ( x < 0.0f ) ? 0.0f : ( x > 1.0f ) ? 1.0f : x;
    }

    // "Gamma compresses" the clamped float result
    int toInt( float x ) const
    {
        return int( pow ( clamp( x ), 1.0f / 2.2f ) * 255.0f + 0.5f );
        //return int( clamp( x ) * 255.0f + 0.5f );
    }

};

#endif /* BUFFER_H_ */
