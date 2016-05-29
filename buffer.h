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

    std::vector< std::vector< std::vector< float > > > buffer_data_;

private:
    inline float clamp( float x ) const
    {
        return ( x < 0.0f ) ? 0.0f : ( x > 1.0f ) ? 1.0f : x;
    }

};

#endif /* BUFFER_H_ */
