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
                        const glm::vec3 background_color,
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

void PathTracer::integrate( void )
{
    Timer t;
    t.start();

    #pragma omp parallel for schedule( dynamic, 1 )

    for ( std::size_t y = 0; y < buffer_.v_resolution_; y++ )
    {
        std::stringstream progress_stream;
        progress_stream << "\rRendering ("
                        << sampler_.size()
                        << std::fixed << std::setw( 6 ) << std::setprecision( 2 )
                        << " spp) : " << 100.0f * y / ( buffer_.v_resolution_ - 1 )
                        << "%";

        std::clog << progress_stream.str();

        for ( std::size_t x = 0; x < buffer_.h_resolution_; x++ )
        {
            // Generate a set of 2D sampling points with coordinates in the interval
            // [-0.5f, +0.5f). The instantiated sampler will define the number and
            // actual distribution (regular, uniform,...) of the points.
            sampler_.generateSamplesCoords();

            for ( std::size_t samp = 0; samp < sampler_.size(); samp++ )
            {
                // Transform a point from the continuous screen space into the 
                // normalized screen space (that ranges from -1.0 to +1.0 along 'x'
                // and 'y'). The value 0.5f is added to the pixel coordinates in
                // screen space in order to have the pixel center as the reference
                // for the sampling points distribution.               
                glm::vec2 pixel_cam_space = glm::vec2{ 2.0f * ( x + 0.5f + sampler_[samp].x ) / buffer_.h_resolution_ - 1.0f,
                                                       2.0f * ( y + 0.5f + sampler_[samp].y ) / buffer_.v_resolution_ - 1.0f };

                // Generates the primary ray in world space from the normalized
                // screen coordinates.
                Ray ray{ camera_.getWorldSpaceRay( pixel_cam_space ) };

                //float xx = x + 0.5f + sampler_[samp].x;
                //float yy = y + 0.5f + sampler_[samp].y;
                //buffer_.buffer_data_[x][y] += 0.5f * ( 1.0f + static_cast< float >( sin( ( xx * xx + yy * yy ) / 100.0f ) ) ); 

                // Trace the ray path.
                buffer_.buffer_data_[x][y] += integrate_recursive( ray, 0 );
            }

            // Compute the average radiance that falls onto the pixel (x,y).
            buffer_.buffer_data_[x][y] /= sampler_.size();
        }
    }
    
    std::clog << std::endl;
    
    t.stop();
    std::clog << "Rendering time: " << t.getElapsedSeconds() << " sec, " << t.getElapsedNanoSeconds() << " nsec." << std::endl;
}

glm::vec3 PathTracer::integrate_recursive( const Ray &ray,
                                          int depth )
{
    IntersectionRecord intersection_record;
    IntersectionRecord tmp_intersection_record;
    std::size_t num_primitives = scene_.primitives_.size();
    glm::vec3 spectrum{ 0.0f, 0.0f, 0.0f };

    intersection_record.t_ = std::numeric_limits< float >::max();

    if ( depth < 5 )
    {
        for ( std::size_t primitive_idx = 0; primitive_idx < num_primitives; primitive_idx++ )
            if ( scene_.primitives_[primitive_idx]->intersect( ray, tmp_intersection_record ) )
                if ( ( tmp_intersection_record.t_ < intersection_record.t_ ) && ( tmp_intersection_record.t_ > 0.0f ) )
                    intersection_record = tmp_intersection_record;

        if ( intersection_record.t_ < std::numeric_limits< float >::max() )
        {
            // flips the normal in the case of a backface in two sided rendering
            if ( glm::dot( intersection_record.normal_, -ray.direction_ ) < 0.0f )
                intersection_record.normal_ = -intersection_record.normal_;
           
            glm::vec3 new_dir = intersection_record.material_.bxdf_.getNewDirection( intersection_record.normal_, rng_ );

            Ray new_ray{ intersection_record.position_ + new_dir * 0.001f, new_dir };

            spectrum = intersection_record.material_.emitted_ + intersection_record.material_.bxdf_.radiance_  *
                                                                integrate_recursive( new_ray, ++depth ) *
                                                                glm::dot( intersection_record.normal_, new_ray.direction_ ) * 2.0f;
        }
        else
            spectrum = background_color_;
    }
    else
        spectrum = background_color_;

    return spectrum;
}

void PathTracer::printInfo( void ) const
{
    std::clog << "PathTracer Renderer Information" << std::endl;
    std::clog << "--------------------------------" << std::endl;

    Integrator::printInfo();
}

