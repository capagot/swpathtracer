#include "buffer.h"

Buffer::Buffer( unsigned int h_resolution,
                unsigned int v_resolution,
                int spectrum_num_samples ) :
        h_resolution_{ h_resolution },
        v_resolution_{ v_resolution },
        spectrum_num_samples_{ spectrum_num_samples }
{
    buffer_data_.resize( h_resolution_, std::vector< glm::dvec3 >( v_resolution_ ) );

    for ( std::size_t y = 0; y < v_resolution_; y++ )
        for ( std::size_t x = 0; x < h_resolution_; x++ )
            buffer_data_[x][y] = glm::dvec3{ 0.0, 0.0, 0.0 };
}

Buffer::~Buffer( void )
{ }

void Buffer::save( const std::string &filename ) const
{
    std::clog << "\n\nBuffer saving started... ";

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
            // convert from real to [0.0, 1.0] with simple clamp, then
            //         gamma compression with gamma = 1/2.2, then
            //         from gamma compressed [0.0, 1.0] to [0.0, 255.0], then
            //         from [0.0, 255.0] to [0.5, 255.5], then
            //         from [0.5, 255.5] to [0, 255] with round to nearest.
            rendering_file << static_cast< int >( pow( clamp( buffer_data_[x][y][0] ), 1 / 2.2 ) * 255.0 + 0.5 ) << " ";
            rendering_file << static_cast< int >( pow( clamp( buffer_data_[x][y][1] ), 1 / 2.2 ) * 255.0 + 0.5 ) << " ";
            rendering_file << static_cast< int >( pow( clamp( buffer_data_[x][y][2] ), 1 / 2.2 ) * 255.0 + 0.5 ) << " ";
        }
    }

    rendering_file.close();

    std::clog << "finished!\n";
}

void Buffer::printInfo( void ) const
{
    std::cout << "Buffer:" << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "  resolution .......................: " << h_resolution_ 
                                                          <<  " x " 
                                                          << v_resolution_ 
                                                          << " ( " << h_resolution_ * v_resolution_ << " ) pixels" << std::endl;
    std::cout << "  pixel format .....................: RGB" << std::endl;
    std::cout << "  color component representation ...: double" << std::endl;
    std::cout << "  gamma compression factor .........: 1 / 2.2 ( ~" << std::fixed << std::setw( 4 ) 
                                                                     << std::setprecision( 2 )
                                                                     << 1.0 / 2.2 << " )" << std::endl;
    std::size_t buffer_byte_size = spectrum_num_samples_ * sizeof( double ) * h_resolution_ * v_resolution_;
    std::clog << "  total byte size ..................: " << buffer_byte_size << " bytes" 
                                                          << " ( ~" 
                                                          << std::fixed << std::setw( 6 )
                                                          << std::setprecision( 2 )
                                                          << buffer_byte_size / 1024.0 / 1024.0 << " MiB )"
                                                          << std::endl;
}

