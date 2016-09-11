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
                        Buffer &buffer,
                        RNG< std::uniform_real_distribution, float, std::mt19937 > &rng ) :
        Integrator{ camera,
                    scene,
                    background_color,
                    max_path_depth,
                    tracing_stop_criterion_,
                    sampler,
                    buffer },
        rng_( rng )
{ }

Spectrum PathTracer::integrate( void )
{
    Timer t;
    t.start();

    #pragma omp parallel for schedule( dynamic, 1 )

    for ( std::size_t y = 0; y < camera_.v_resolution_; y++ )
    {
        std::stringstream progress_stream;
        progress_stream << "\rRendering ("
                        << sampler_.size()
                        << std::fixed << std::setw( 6 ) << std::setprecision( 2 )
                        << " spp) : " << 100.0f * y / ( camera_.v_resolution_ - 1 )
                        << "%";

        std::clog << progress_stream.str();

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
    }

    t.stop();
    std::clog << "Rendering time: " << t.getElapsedSeconds() << " sec, " << t.getElapsedNanoSeconds() << " nsec." << std::endl;
}

Spectrum PathTracer::integrate_recursive( const Ray &ray,
                                          int depth )
{
    IntersectionRecord intersection_record;
    IntersectionRecord tmp_intersection_record;
    std::size_t num_primitives = scene_.primitives_.size();
    Spectrum spectrum{ glm::vec3{ 0.0f, 0.0f, 0.0f } };

    intersection_record.t_ = std::numeric_limits< float >::max();

    int closest_primitive_idx = -1;

    if ( depth < 5 )
    {
        for ( std::size_t primitive_idx = 0; primitive_idx < num_primitives; primitive_idx++ )
            if ( scene_.primitives_[primitive_idx]->intersect( ray, tmp_intersection_record ) )
                if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0f ) )
                //if ( tmp_intersection_record.t_ < intersection_record.t_ )
                {
                    intersection_record = tmp_intersection_record;
                    closest_primitive_idx = primitive_idx;
                }

        if ( intersection_record.t_ < std::numeric_limits< float >::max() )
        {
            // flips the normal in the case of a backface in two sided rendering
            if ( glm::dot( intersection_record.normal_, -ray.direction_ ) < 0.0f )
                intersection_record.normal_ = -intersection_record.normal_;

            //*/

            // diffuse color
            //spectrum.spectrum_ = intersection_record.material_.brdf_.spectrum_  +  intersection_record.material_.emitted_.spectrum_;

            // normal vector
            //spectrum.spectrum_ = -intersection_record.normal_;
            
            // normal vector length
            //spectrum.spectrum_ =  glm::vec3( (glm::length( intersection_record.normal_ ) == 1.0f )? 1.0 : 0.0, 0.0f, 0.0f );

            // N . ray (cos theta) X diffuse color
            //spectrum.spectrum_ = glm::vec3{ glm::dot( intersection_record.normal_, -ray.direction_ ) } *
            //                     ( intersection_record.material_.brdf_.spectrum_  +  intersection_record.material_.emitted_.spectrum_ );

           
            glm::vec3 new_dir = intersection_record.material_.brdf_.getDirection( intersection_record.normal_, rng_ );

            Ray new_ray{ intersection_record.position_ + new_dir * 0.001f, new_dir };

            spectrum.spectrum_ = intersection_record.material_.emitted_.spectrum_ + ( intersection_record.material_.brdf_.spectrum_  / static_cast< float >( M_PI ) ) *
                                                                                      integrate_recursive( new_ray, ++depth ).spectrum_ *
                                                                                      glm::dot( intersection_record.normal_, new_ray.direction_ ) *
                                                                                      2.0f * static_cast< float >( M_PI );
        }
        else
            spectrum.spectrum_ = background_color_.spectrum_;
    }
    else
        spectrum.spectrum_ = background_color_.spectrum_;

    return spectrum;
}

void PathTracer::printInfo( void ) const
{
    std::clog << "PathTracer Renderer Information" << std::endl;
    std::clog << "--------------------------------" << std::endl;

    Integrator::printInfo();
}
