/*
 * renderer.cpp
 *
 *  Created on: May 13, 2016
 *      Author: christian
 */

#include "renderer.h"

Renderer::Renderer( const Camera &camera,
                    const Scene &scene,
                    Buffer &buffer,
                    unsigned int spp,
                    PixelSamplingMethod pixel_sampling_method,
                    unsigned int max_path_depth,
                    TracingStoppingCriterion tracing_stop_criterion,
                    const Spectrum background_color ) :
        camera_( camera ),
        scene_( scene ),
        buffer_( buffer ),
        spp_{ spp },
        pixel_sampling_method_{ pixel_sampling_method },
        max_path_depth_{ max_path_depth },
        tracing_stop_criterion_{ tracing_stop_criterion },
        background_color_{ background_color }
{ }

// TODO: consider to make getters inline
unsigned int Renderer::getSPP( void ) const
{
    return spp_;
}

Renderer::PixelSamplingMethod Renderer::getPixelSamplingMethod( void ) const
{
    return pixel_sampling_method_;
}

unsigned int Renderer::getMaxPathDepth( void ) const
{
    return max_path_depth_;
}

Renderer::TracingStoppingCriterion Renderer::getTracingStopCriterion( void ) const
{
    return tracing_stop_criterion_;
}

void Renderer::printInfo( void ) const
{
    std::clog << " spp_ ..........................: " << spp_ << std::endl;
    std::clog << " pixel_sampling_method_ ........: ";

    switch( pixel_sampling_method_ )
    {
    case UNIFORM_SAMPLING:
        std::clog << "UNIFORM_SAMPLING" << std::endl;
        break;
    case REGULAR_SAMPLING:
        std::clog << "REGULAR_SAMPLING" << std::endl;
        break;
    case JITTERED_SAMPLING:
        std::clog << "JITTERED_SAMPLING" << std::endl;
        break;
    case POISSON_SAMPLING:
        std::clog << "POISSON_SAMPLING" << std::endl;
        break;
    }

    std::clog << " max_path_depth_ ...............: " << max_path_depth_ << std::endl;

    std::clog << " tracing_stop_criterion_ .......: ";

    switch( tracing_stop_criterion_ )
    {
    case MAX_DEPTH:
        std::clog << "MAX_DEPTH" << std::endl;
        break;
    case RUSSIAN_ROULETTE:
        std::clog << "RUSSIAN_ROULETTE" << std::endl;
        break;
    }
}

