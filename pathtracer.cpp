#include "pathtracer.h"
#include "timer.h"

PathTracer::PathTracer( Camera &camera,
                        const Scene &scene,
                        const glm::dvec3 background_color,
                        unsigned int max_path_depth,
                        TracingStoppingCriterion tracing_stop_criterion_,
                        Sampler &sampler,
                        Buffer &buffer,
                        RNG< std::uniform_real_distribution, double, std::mt19937 > &rng ) :
        Integrator{ camera,
                    scene,
                    background_color,
                    max_path_depth,
                    tracing_stop_criterion_,
                    sampler,
                    buffer },
        rng_( rng )
{
    int num_threads = std::max( 1, omp_get_max_threads() );
    for( int thread_id = 0; thread_id < num_threads; thread_id++ )
    {
        num_rays_.push_back( 0 );
        num_intersection_tests_.push_back( 0 );
        num_intersections_.push_back( 0 );
    }
}

void PathTracer::integrate( void )
{
    // TODO: make t (time) a data member
    Timer t;
    t.start();

    #pragma omp parallel for schedule( dynamic, 1 )

    for ( std::size_t y = 0; y < buffer_.v_resolution_; y++ )
    {
        std::stringstream progress_stream;
        progress_stream << "\r  progress .........................: "
                        << std::fixed << std::setw( 6 )
                        << std::setprecision( 2 )
                        << 100.0 * y / ( buffer_.v_resolution_ - 1 )
                        << "%";

        int thread_id = omp_get_thread_num();

        std::clog << progress_stream.str();

        for ( std::size_t x = 0; x < buffer_.h_resolution_; x++ )
        {
            // Generate a set of 2D sampling points with coordinates in the interval
            // [-0.5, +0.5). The instantiated sampler will define the number and
            // actual sample distribution (regular, uniform,...).
            sampler_.generateSamplesCoords();

            // DEBUG code
            //if ( x == 100 && y == 50 )
            //{


            for ( std::size_t samp = 0; samp < sampler_.size(); samp++ )
            {
                // Transform a point from the continuous screen space into the
                // normalized screen space (that ranges from -1.0 to +1.0 along 'x'
                // and 'y'). The value 0.5 is added to the pixel coordinates in
                // screen space in order to have the pixel center as the reference
                // for the sampling points distribution.
                glm::dvec2 pixel_cam_space = glm::dvec2{ 2.0 * ( x + 0.5 + sampler_[samp].x ) / buffer_.h_resolution_ - 1.0,
                                                         2.0 * ( y + 0.5 + sampler_[samp].y ) / buffer_.v_resolution_ - 1.0 };

                // Generate the primary ray in world space from the normalized
                // screen coordinates.
                Ray ray{ camera_.getWorldSpaceRay( pixel_cam_space ) };

                // Trace the ray path.
                buffer_.buffer_data_[x][y] += integrate_recursive( ray, 0, thread_id );
            }

            //}
            //else
            //    buffer_.buffer_data_[x][y] = glm::dvec3{ 0.0, 0.0, 0.0 };

            // Compute the average radiance that falls onto the pixel (x,y).
            buffer_.buffer_data_[x][y] /= sampler_.size();
        }
    }

    std::clog << std::endl;

    t.stop();
    std::clog << "  total rendering time .............: " << t.getElapsedSeconds() << " sec, " << t.getElapsedNanoSeconds() << " nsec." << std::endl;
}

glm::dvec3 PathTracer::integrate_recursive( const Ray &ray,
                                            unsigned int depth,
                                            int thread_id )
{
    IntersectionRecord intersection_record;
    glm::dvec3 spectrum{ 0.0, 0.0, 0.0 };
    std::vector< glm::dvec3 > w_r;
    intersection_record.t_ = std::numeric_limits< double >::max();

    if ( !scene_.intersect( ray,
                            intersection_record,
                            num_intersection_tests_[ thread_id ],
                            num_intersections_[ thread_id ] ) )
    {
        if ( !depth )
            return background_color_;
        else
            return spectrum;
    }

    ONB tangent_frame;
    tangent_frame.setFromV( intersection_record.normal_ );

    glm::dmat3x3 tangent_to_universe_space = tangent_frame.getBasisMatrix();
    glm::dmat3x3 universe_to_tangent_space = glm::transpose( tangent_to_universe_space );

    glm::dvec3 w_i = universe_to_tangent_space * -ray.direction_;

    scene_.materials_[intersection_record.material_id_]->bsdf_->getNewDirection( w_i, w_r );

    glm::dvec3 new_dir = tangent_to_universe_space * w_r[ w_r.size() - 1 ];
    Ray new_ray{ intersection_record.position_ + new_dir * 0.00001, new_dir };

    glm::dvec3 fr = scene_.materials_[intersection_record.material_id_]->bsdf_->fr( w_i, w_r );

    double p = std::max( std::max( fr[0], fr[1] ), fr[2] );

    if ( depth >= max_path_depth_ )
    {
        if ( rng_() < p )
            fr *= 1.0 / p;
        else
            return scene_.materials_[intersection_record.material_id_]->emitted_;
    }

    num_rays_[ thread_id ]++;

    spectrum = scene_.materials_[intersection_record.material_id_]->emitted_ +
        fr *
        integrate_recursive( new_ray, ++depth, thread_id );// *
        //w_r[ w_r.size() - 1 ].y;

    return spectrum;
}

void PathTracer::printInfoPreRendering( void ) const
{
    std::cout << "  # of threads .....................: " << omp_get_max_threads() << std::endl;
    std::cout << "  rendering algorithm ..............: brute force path tracing" << std::endl;
}

void PathTracer::printInfoPostRendering( void ) const
{
    long unsigned int num_rays = 0;
    long unsigned int num_intersection_tests = 0;
    long unsigned int num_intersections = 0;

    for ( unsigned int thread_id = 0; thread_id < num_rays_.size(); thread_id++ )
    {
        num_rays += num_rays_[thread_id];
        num_intersection_tests += num_intersection_tests_[thread_id];
        num_intersections += num_intersections_[thread_id];
    }

    std::cout << "  total # of rays ..................: " << num_rays << std::endl;
    std::cout << "  total # of intersection tests ....: " << num_intersection_tests << std::endl;
    std::cout << "  total # of intersections .........: " << num_intersections
                                                          << " ( ~"
                                                          << std::fixed << std::setw( 6 )
                                                          << std::setprecision( 2 )
                                                          << static_cast< double >( num_intersections ) / num_intersection_tests * 100.0
                                                          << "% )"
                                                          << std::endl;
}
