/*
 * sampler.h
 *
 *  Created on: Jul 5, 2016
 *      Author: christian
 */

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

    virtual void generateSamplesCoords( const glm::vec2 &pixel_coord ) = 0;

    glm::vec2 operator[]( std::size_t i ) const
    {
        int thread_id = omp_get_thread_num();
        return samples_[thread_id][i];
    }

    std::size_t size( void )
    {
        int thread_id = omp_get_thread_num();
        return spp_;
    }

    std::size_t spp_;
    std::vector< std::vector< glm::vec2 > > samples_;
};

class UniformSampler : public Sampler
{
public:

    UniformSampler( std::size_t spp )
    {
        spp_ = spp;
        int num_threads = std::max( 1, omp_get_max_threads() );
        for( int thread_count = 0; thread_count < num_threads; thread_count++ )
            samples_[thread_count].resize( spp_ );
    }

    void generateSamplesCoords( const glm::vec2 &pixel_coord )
    {
        int thread_id = omp_get_thread_num();
        for( unsigned int i = 0; i < spp_; i++ )
            samples_[thread_id][i] = glm::vec2{ pixel_coord.x + random_(), pixel_coord.y + random_() };
    }

private:

    RNG< std::uniform_real_distribution, float, std::mt19937 > random_{ 0.0f, 1.0f };
};

class RegularSampler : public Sampler
{
public:

    RegularSampler( std::size_t spp )
    {
        num_subpixels_x_ = static_cast< unsigned int >( ceil( sqrt( spp ) ) );
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

    void generateSamplesCoords( const glm::vec2 &pixel_coord )
    {
        int thread_id = omp_get_thread_num();
        std::size_t i = 0;

        for ( std::size_t subpixel_x_count = 0; subpixel_x_count < num_subpixels_x_ ; subpixel_x_count++ )
            for ( std::size_t subpixel_y_count = 0; subpixel_y_count < num_subpixels_y_ ; subpixel_y_count++ )
            {
                samples_[thread_id][i] = glm::vec2{ pixel_coord.x + subpixel_x_count * subpixel_width_+ subpixel_half_width_,
                                                    pixel_coord.y + subpixel_y_count * subpixel_height_+ subpixel_half_height_};
                i++;
            }
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

    JitteredSampler( std::size_t spp )
    {
        num_subpixels_x_ = static_cast< unsigned int >( ceil( sqrt( spp ) ) );
        num_subpixels_y_ = num_subpixels_x_;
        spp_ = num_subpixels_y_ * num_subpixels_x_;
        subpixel_width_ = 1.0f / num_subpixels_x_;
        subpixel_height_ = 1.0f / num_subpixels_y_;

        int num_threads = std::max( 1, omp_get_max_threads() );
        for( int thread_count = 0; thread_count < num_threads; thread_count++ )
            samples_[thread_count].resize( spp_ );
    }

    void generateSamplesCoords( const glm::vec2 &pixel_coord )
    {
        int thread_id = omp_get_thread_num();
        std::size_t i = 0;

        for ( std::size_t subpixel_x_count = 0; subpixel_x_count < num_subpixels_x_ ; subpixel_x_count++ )
            for ( std::size_t subpixel_y_count = 0; subpixel_y_count < num_subpixels_y_ ; subpixel_y_count++ )
            {
                samples_[thread_id][i] = glm::vec2{ pixel_coord.x + subpixel_x_count * subpixel_width_+ random_(),
                                                    pixel_coord.y + subpixel_y_count * subpixel_height_+ random_() };
                i++;
            }
    }

private:

    RNG< std::uniform_real_distribution, float, std::mt19937 > random_{ 0.0f, 1.0f };
    std::size_t num_subpixels_x_;       // number of subpixels along x axis
    std::size_t num_subpixels_y_;       // number of subpixels along y axis
    float subpixel_width_;              // width of the subpixel (1 / pixel_width)
    float subpixel_height_;             // height of the subpixel (1 / pixel_height)

};

#endif /* SAMPLER_H_ */
