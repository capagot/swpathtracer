#ifndef BUFFER_H_
#define BUFFER_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

class Buffer
{
public:

    Buffer( unsigned int h_resolution,
            unsigned int v_resolution,
            int spectrum_num_samples );

    ~Buffer( void );

    void save( const std::string &filename ) const;

    void printInfo( void ) const;

    unsigned int h_resolution_ = 512;

    unsigned int v_resolution_ = 512;

    int spectrum_num_samples_ = 3;

    std::vector< std::vector< glm::vec3 > > buffer_data_;

private:

    float clamp( float x ) const
    {
        return ( x < 0.0f ) ? 0.0f : ( x > 1.0f ) ? 1.0f : x;
    }
};

#endif /* BUFFER_H_ */

