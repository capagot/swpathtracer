/*
 * pathtracer.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: christian
 */

#include "pathtracer.h"

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
    std::size_t primitive_idx;
    IntersectionRecord tmp_intersection_record;
    IntersectionRecord intersection_record;

    // TODO: enable OpenMP
    //#pragma omp parallel for private ( x, y )

    for ( y = 0; y < camera_.v_resolution_; y++ )
        for ( x = 0; x < camera_.h_resolution_; x++ )
        {
            Ray ray( camera_.getRay( x, y ) );

            intersection_record.t_ = std::numeric_limits< float >::max();

            // loops over object's primitives
            std::size_t num_primitives = 2;// scene_.primitives_.size();

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

    std::clog << "finished!";
}

void PathTracer::printInfo( void ) const
{
    std::clog << "PathTracer Renderer Information" << std::endl;
    std::clog << "--------------------------------" << std::endl;

    Renderer::printInfo();
}
