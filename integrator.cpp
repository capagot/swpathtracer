/*
 * integrator.cpp
 *
 *  Created on: May 13, 2016
 *      Author: christian
 */

#include "integrator.h"

Integrator::Integrator( Camera &camera,
                        const Scene &scene,
                        const Spectrum background_color,
                        unsigned int max_path_depth,
                        TracingStoppingCriterion tracing_stop_criterion_,
                        Sampler &sampler,
                        Buffer &buffer ) :
        camera_( camera ),
        scene_( scene ),
        background_color_{ background_color },
        max_path_depth_{ max_path_depth },
        tracing_stop_criterion_{ tracing_stop_criterion_ },
        sampler_( sampler ),
        buffer_( buffer )
{ }

void Integrator::printInfo( void ) const
{
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
