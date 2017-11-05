#ifndef SAMPLER_H_
#define SAMPLER_H_

#include <vector>

#include "random.h"

class Sampler
{
public:

    Sampler( void )
    {
        int num_threads = std::max( 1, omp_get_max_threads() );
        for( int thread_count = 0; thread_count < num_threads; thread_count++ )
            samples_.push_back( std::vector< glm::vec2 >() );
    }

    virtual ~Sampler( void )
    { };

    virtual void generateSamplesCoords( void ) = 0;

    glm::vec2 operator[]( std::size_t i ) const
    {
        int thread_id = omp_get_thread_num();
        return samples_[thread_id][i];
    }

    std::size_t size( void )
    {
        return spp_;
    }

    virtual void printInfo( void ) const = 0;

    std::size_t spp_;

    std::vector< std::vector< glm::vec2 > > samples_;
};

// Generate a set of (x, y) samples uniformly distributed, such that x and y are in the interval [-1, +1).
// The number of samples is given by the 'spp_' data member.
class UniformSampler : public Sampler
{
public:

    // A separate sample storage buffer is instantiated for each thread, in order to avoid race condition
    // during the sample generation stage.
    UniformSampler( RNG< std::uniform_real_distribution, float, std::mt19937 > &rng, std::size_t spp ) :
        rng_( rng )
    {
        spp_ = spp;
        int num_threads = std::max( 1, omp_get_max_threads() );
        for( int thread_count = 0; thread_count < num_threads; thread_count++ )
            samples_[thread_count].resize( spp_ );
    }

    ~UniformSampler( void )
    { }

    void generateSamplesCoords( void )
    {
        int thread_id = omp_get_thread_num();
        for( unsigned int i = 0; i < spp_; i++ )
            samples_[thread_id][i] = glm::vec2{ rng_() - 0.5f, rng_() - 0.5f };
    }

    void printInfo( void ) const
    {
        std::cout << "Sampler: " << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << "  type .............................: uniform" << std::endl;
        std::cout << "  samples per pixel ................: " << samples_[0].size() << std::endl;
    }

private:

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;
};

class RegularSampler : public Sampler
{
public:

    RegularSampler( std::size_t spp )
    {
        num_subpixels_x_ = static_cast< unsigned int >( ceil( sqrtf( spp ) ) );
        num_subpixels_y_ = num_subpixels_x_;
        spp_ = num_subpixels_y_ * num_subpixels_x_;
        subpixel_width_ = 1.0f / num_subpixels_x_;
        subpixel_height_ = 1.0f / num_subpixels_y_;
        subpixel_half_width_ = subpixel_width_ * 0.5f;
        subpixel_half_height_ = subpixel_height_ * 0.5f;

        int num_threads = std::max( 1, omp_get_max_threads() );
        for( int thread_count = 0; thread_count < num_threads; thread_count++ )
            samples_[thread_count].resize( spp_ );
    }

    ~RegularSampler( void )
    { }

    void generateSamplesCoords( void )
    {
        int thread_id = omp_get_thread_num();
        std::size_t i = 0;

        for ( std::size_t subpixel_x_count = 0; subpixel_x_count < num_subpixels_x_ ; subpixel_x_count++ )
            for ( std::size_t subpixel_y_count = 0; subpixel_y_count < num_subpixels_y_ ; subpixel_y_count++ )
            {
                samples_[thread_id][i] = glm::vec2{ subpixel_x_count * subpixel_width_+ subpixel_half_width_ - 0.5f,
                                                    subpixel_y_count * subpixel_height_+ subpixel_half_height_ - 0.5f };
                i++;
            }
    }

    void printInfo( void ) const
    {
        std::cout << "Sampler: " << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << "  type .............................: regular" << std::endl;
        std::cout << "  samples per pixel ................: " << samples_[0].size() << std::endl;
    }

private:

    std::size_t num_subpixels_x_;       // number of subpixels along x axis
    std::size_t num_subpixels_y_;       // number of subpixels along y axis
    float subpixel_width_;              // width of the subpixel (1 / pixel_width)
    float subpixel_height_;             // height of the subpixel (1 / pixel_height)
    float subpixel_half_width_;         // ( width of the subpixel ) / 2
    float subpixel_half_height_;        // ( height of the subpixel ) / 2
};

class JitteredSampler : public Sampler
{
public:

    JitteredSampler( RNG< std::uniform_real_distribution, float, std::mt19937 > &rng, std::size_t spp ) :
        rng_( rng )
    {
        num_subpixels_x_ = static_cast< unsigned int >( ceil( sqrtf( spp ) ) );
        num_subpixels_y_ = num_subpixels_x_;
        spp_ = num_subpixels_y_ * num_subpixels_x_;
        subpixel_width_ = 1.0f / num_subpixels_x_;
        subpixel_height_ = 1.0f / num_subpixels_y_;

        int num_threads = std::max( 1, omp_get_max_threads() );
        for( int thread_count = 0; thread_count < num_threads; thread_count++ )
            samples_[thread_count].resize( spp_ );
    }

    ~JitteredSampler( void )
    { }

    void generateSamplesCoords( void )
    {
        int thread_id = omp_get_thread_num();
        std::size_t i = 0;

        for ( std::size_t subpixel_x_count = 0; subpixel_x_count < num_subpixels_x_ ; subpixel_x_count++ )
            for ( std::size_t subpixel_y_count = 0; subpixel_y_count < num_subpixels_y_ ; subpixel_y_count++ )
            {
                samples_[thread_id][i] = glm::vec2{ subpixel_x_count * subpixel_width_+ subpixel_width_ * rng_() - 0.5f,
                                                    subpixel_y_count * subpixel_height_ + subpixel_height_ * rng_() - 0.5f };
                i++;
            }
    }

    void printInfo( void ) const
    {
        std::cout << "Sampler: " << std::endl;
        std::cout << "-------------------------------------------------------------------------------" << std::endl;
        std::cout << "  type .............................: jittered" << std::endl;
        std::cout << "  samples per pixel ................: " << samples_[0].size() << std::endl;
    }

private:

    RNG< std::uniform_real_distribution, float, std::mt19937 > rng_;
    std::size_t num_subpixels_x_;       // number of subpixels along x axis
    std::size_t num_subpixels_y_;       // number of subpixels along y axis
    float subpixel_width_;              // width of the subpixel (1 / pixel_width)
    float subpixel_height_;             // height of the subpixel (1 / pixel_height)
};

#endif /* SAMPLER_H_ */

