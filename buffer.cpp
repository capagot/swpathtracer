/*
 * buffer.cpp
 *
 *  Created on: Apr 22, 2016
 *      Author: christian
 */

#include "buffer.h"

Buffer::Buffer( unsigned int h_resolution,
                unsigned int v_resolution,
                int spectrum_num_samples ) :
        h_resolution_{ h_resolution },
        v_resolution_{ v_resolution },
        spectrum_num_samples_{ spectrum_num_samples }
{
    buffer_data_.resize( h_resolution_, std::vector< Spectrum >( v_resolution_ ) );

    for ( std::size_t y = 0; y < v_resolution_; y++ )
        for ( std::size_t x = 0; x < h_resolution_; x++ )
            buffer_data_[x][y].spectrum_ = glm::vec3{ 0.0f, 0.0f, 0.0f };
}

Buffer::~Buffer( void )
{ }

void Buffer::printInfo( void ) const
{
    std::clog << "Rendering Buffer Information" << std::endl;
    std::clog << "----------------------------" << std::endl;

    std::clog << " h_resolution_ ..................: " << h_resolution_ << std::endl;
    std::clog << " v_resolution_ ..................: " << v_resolution_ << std::endl;
    std::clog << " spectrum_num_samples_ ..........: " << spectrum_num_samples_ << std::endl;

    std::size_t buffer_byte_size = spectrum_num_samples_ * sizeof( float ) * h_resolution_ * v_resolution_;

    std::clog << " buffer total byte size .........: " << buffer_byte_size << std::endl;
}

// TODO: Currently, the rendered image is saved in .ppm format.
//       I have to change this to the EXR format.
void Buffer::save( const std::string &filename ) const
{
    std::clog << "Buffer saving started... ";

    std::ofstream rendering_file;

    rendering_file.open( filename.c_str() );

    rendering_file << "P3"
                   << std::endl
                   << h_resolution_
                   << " "
                   << v_resolution_
                   << std::endl
                   << 255
                   << std::endl;

    for ( unsigned int y = 0; y < v_resolution_; y++ )
    {
        for ( unsigned int x = 0; x < h_resolution_; x++ )
        {
            // convert from real to [0.0f, 1.0f] with simple clamp, then
            //         from [0.0f, 1.0f] to [0.0f, 255.0f], then
            //         from [0.0f, 255.0f] to [0.5f, 255.5f], then
            //         from [0.5f, 255.5f] to [0, 255] with round to nearest.
            rendering_file << static_cast< int >( clamp( buffer_data_[x][y].spectrum_[0] ) * 255.0f + 0.5f ) << " ";
            rendering_file << static_cast< int >( clamp( buffer_data_[x][y].spectrum_[1] ) * 255.0f + 0.5f ) << " ";
            rendering_file << static_cast< int >( clamp( buffer_data_[x][y].spectrum_[2] ) * 255.0f + 0.5f ) << " ";
        }
        //rendering_file << std::endl;
    }

    rendering_file.close();

    std::clog << "finished!";
}
