/*
 * pathtracer.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: christian
 */

#include "pathtracer.h"
#include "timer.h"

PathTracer::PathTracer( const Camera &camera,
                        const Scene &scene,
                        Buffer &buffer,
                        const Spectrum background_color,
                        unsigned int spp,
                        PixelSamplingMethod pixel_sampling_method,
                        unsigned int max_path_depth,
                        TracingStoppingCriterion stop_criterion ) :
        Renderer::Renderer{ camera,
                            scene,
                            buffer,
                            spp,
                            pixel_sampling_method,
                            max_path_depth,
                            stop_criterion,
                            background_color }
{ }

void PathTracer::render( void )
{
    std::clog << "Rendering started... ";

    std::size_t x;
    std::size_t y;
    IntersectionRecord intersection_record;
    IntersectionRecord tmp_intersection_record;
    std::size_t primitive_idx;

    std::size_t num_primitives = scene_.primitives_.size();

    Timer t;
    t.start();

    #pragma omp parallel for schedule(dynamic, 1) private ( y, x, intersection_record, tmp_intersection_record, primitive_idx )

    for ( y = 0; y < camera_.v_resolution_; y++ )
    {
        for ( x = 0; x < camera_.h_resolution_; x++ )
        {
            Ray ray( camera_.getRay( x, y ) );

            intersection_record.t_ = std::numeric_limits< float >::max();

            for ( primitive_idx = 0; primitive_idx < num_primitives; primitive_idx++ )
                if ( scene_.primitives_[primitive_idx]->intersect( ray, tmp_intersection_record ) )
                    if ( tmp_intersection_record.t_ < intersection_record.t_ )
                        intersection_record = tmp_intersection_record;

            if( intersection_record.t_ < std::numeric_limits< float >::max() )
            {
                buffer_.buffer_data_[x][y][0] = intersection_record.spectrum_.spectrum_[0] * intersection_record.t_ * 0.2f;
                buffer_.buffer_data_[x][y][1] = intersection_record.spectrum_.spectrum_[1] * intersection_record.t_ * 0.2f;
                buffer_.buffer_data_[x][y][2] = intersection_record.spectrum_.spectrum_[2] * intersection_record.t_ * 0.2f;
            }
            else
            {
                buffer_.buffer_data_[x][y][0] = background_color_.spectrum_[0];
                buffer_.buffer_data_[x][y][1] = background_color_.spectrum_[1];
                buffer_.buffer_data_[x][y][2] = background_color_.spectrum_[2];
            }
        }
    }

    t.stop();

    std::clog << "finished!" << std::endl;
    std::clog << "Rendering time: " << t.getElapsedSeconds() << " sec, " << t.getElapsedNanoSeconds() << " nsec." << std::endl;
}

void PathTracer::printInfo( void ) const
{
    std::clog << "PathTracer Renderer Information" << std::endl;
    std::clog << "--------------------------------" << std::endl;

    Renderer::printInfo();
}
