/*
 * pathtracer.cpp
 *
 *  Created on: Apr 24, 2016
 *      Author: christian
 */

#include "pathtracer.h"
#include "timer.h"

PathTracer::PathTracer( Camera &camera,
                        const Scene &scene,
                        const Spectrum background_color,
                        unsigned int max_path_depth,
                        TracingStoppingCriterion tracing_stop_criterion_,
                        Sampler &sampler,
                        Buffer &buffer ) :
        Integrator{ camera,
                    scene,
                    background_color,
                    max_path_depth,
                    tracing_stop_criterion_,
                    sampler,
                    buffer }
{ }

Spectrum PathTracer::integrate( void )
{
    Timer t;
    t.start();

    #pragma omp parallel for schedule(dynamic, 1)

    for ( std::size_t y = 0; y < camera_.v_resolution_; y++ )
        for ( std::size_t x = 0; x < camera_.h_resolution_; x++ )
        {
            sampler_.generateSamplesCoords( glm::vec2{ x, y } );

            for ( std::size_t samp = 0; samp < sampler_.size(); samp++ )
            {
                Ray ray( camera_.getRay( sampler_[samp] ) );
                //buffer_.buffer_data_[x][y].spectrum_ += glm::vec3( (0.5f * ( sin( ( sampler_[samp].x*sampler_[samp].x + sampler_[samp].y*sampler_[samp].y ) / 100.0f ) ) ) );
                buffer_.buffer_data_[x][y].spectrum_ += integrate_recursive( ray, 0 ).spectrum_;
            }

            buffer_.buffer_data_[x][y].spectrum_ /= sampler_.size();
        }

    t.stop();
    std::clog << "Rendering time: " << t.getElapsedSeconds() << " sec, " << t.getElapsedNanoSeconds() << " nsec." << std::endl;
}

Spectrum PathTracer::integrate_recursive( const Ray &ray,
                                          int depth )
{
    IntersectionRecord intersection_record;
    IntersectionRecord tmp_intersection_record;
    std::size_t primitive_idx;
    std::size_t num_primitives = scene_.primitives_.size();
    Spectrum spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } };

    intersection_record.t_ = std::numeric_limits< float >::max();

    for ( primitive_idx = 0; primitive_idx < num_primitives; primitive_idx++ )
        if ( scene_.primitives_[primitive_idx]->intersect( ray, tmp_intersection_record ) )
            if ( tmp_intersection_record.t_ < intersection_record.t_ )
                intersection_record = tmp_intersection_record;

    if ( intersection_record.t_ < std::numeric_limits< float >::max() )
        spectrum.spectrum_ +=  intersection_record.material_.brdf_.spectrum_ * (intersection_record.t_ * 0.1f)  + intersection_record.material_.emitted_.spectrum_ ;
    else
        spectrum.spectrum_ += background_color_.spectrum_;

    //if ( intersection_record.t_ < std::numeric_limits< float >::max() )
    //{
    //    Ray ray; // compute reflected ray
    //    spectrum.spectrum_ += intersection_record.material_.emitted_.spectrum_ + intersection_record.material_.brdf_.spectrum_ * render_recursive( ray, depth + 1, xi ).spectrum_;
    //}
    //else
    //    spectrum.spectrum_ += background_color_.spectrum_;
    //*/

    //return Spectrum{ glm::vec3{ 0.0f, 0.0f, 1.0f } };//spectrum;
    return spectrum;
}

void PathTracer::printInfo( void ) const
{
    std::clog << "PathTracer Renderer Information" << std::endl;
    std::clog << "--------------------------------" << std::endl;

    Integrator::printInfo();
}
