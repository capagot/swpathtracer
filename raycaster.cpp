#include "raycaster.h"
#include "timer.h"

RayCaster::RayCaster( Camera &camera,
                      const Scene &scene,
                      const glm::vec3 background_color,
                      PathTerminationCriterion path_termination_criterion,
                      unsigned int path_length,
                      Sampler &sampler,
                      Buffer &buffer,
                      RNG< std::uniform_real_distribution, float, std::mt19937 > &rng ) :
        Integrator{ camera,
                    scene,
                    background_color,
                    path_termination_criterion,
                    path_length,
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

void RayCaster::integrate( void )
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
                        << 100.0f * y / ( buffer_.v_resolution_ - 1 )
                        << "%";

        int thread_id = omp_get_thread_num();

        std::clog << progress_stream.str();

        for ( std::size_t x = 0; x < buffer_.h_resolution_; x++ )
        {
            glm::vec2 pixel_cam_space = glm::vec2{ 2.0f * ( x + 0.5f ) / buffer_.h_resolution_ - 1.0f,
                                                   2.0f * ( y + 0.5f ) / buffer_.v_resolution_ - 1.0f };

            Ray ray{ camera_.getWorldSpaceRay( pixel_cam_space ) };

            buffer_.buffer_data_[x][y] = integrate_recursive( ray, 0, thread_id );
        }
    }

    float max_t = 0.0f;

    for ( std::size_t y = 0; y < buffer_.v_resolution_; y++ )
        for ( std::size_t x = 0; x < buffer_.h_resolution_; x++ )
            max_t = ( max_t > buffer_.buffer_data_[x][y][0] )? max_t : buffer_.buffer_data_[x][y][0];


        std::cout << "\n\nactual max_t: " << max_t << "\n";
        //max_t = 895.0f;
        //max_t = 161.0f;

    std::cout << "\n---------> Max Intersect/ray: " << max_t << "\n\n";

    for ( std::size_t y = 0; y < buffer_.v_resolution_; y++ )
    {
        for ( std::size_t x = 0; x < buffer_.h_resolution_; x++ )
        {
            float t = buffer_.buffer_data_[x][y][0] / max_t;

            float r, g, b = 0.0f;

            float h = 0.65f - (t  * 0.65f);
            float s = 1.0f;
            float l = t  * 0.5f;

            float v1;
            float v2;

            if ( s == 0.0 ) // s in (0.0, 1.0)
               r = g = b = static_cast< int >( l * 255.0f );
            else {
               if ( l < 0.5f ) {
                   v2 = l * ( 1.0f + s );
               }
               else {
                   v2 = ( l + s ) - ( s * l );
               }

               v1 = 2.0f * l - v2;

               r = Hue2RGB( v1, v2, h + ( 1.0f / 3.0f ) );
               g = Hue2RGB( v1, v2, h );
               b = Hue2RGB( v1, v2, h - ( 1.0f / 3.0f ) );
            }

            buffer_.buffer_data_[x][y] = glm::vec3{ r, g, b };
        }
    }

    std::clog << std::endl;

    t.stop();
    std::clog << "  total rendering time .............: " << t.getElapsedSeconds() << " sec, " << t.getElapsedNanoSeconds() << " nsec." << std::endl;
}

float RayCaster::Hue2RGB( float v1, float v2, float h )
{
   if ( h < 0.0f )
       h += 1.0f;

   if ( h > 1.0f )
       h -= 1.0f;

   if ( ( 6.0f * h ) < 1.0f )
       return v1 + ( v2 - v1 ) * 6.0f * h;

   if ( ( 2.0f * h ) < 1.0f )
       return v2;

   if ( ( 3.0f * h ) < 2.0f )
       return v1 + ( v2 - v1 ) * ( ( 2.0f / 3.0f ) - h ) * 6.0f;

   return v1;
}

glm::vec3 RayCaster::integrate_recursive( const Ray &ray,
                                            unsigned int depth,
                                            int thread_id )
{
    IntersectionRecord intersection_record;
    intersection_record.t_ = std::numeric_limits< float >::max();

	glm::vec3 color = glm::vec3{ 1.0f, 0.0f, 0.0f };

    long unsigned int num_intersection_tests = 0;
    long unsigned int num_intersections = 0;

    scene_.intersect( ray,
                      intersection_record,
                      num_intersection_tests,
                      num_intersections );

    num_intersection_tests_[ thread_id ] += num_intersection_tests;
    num_intersections_[ thread_id ] += num_intersections;

	return glm::vec3{ num_intersection_tests, 0.0f, 0.0f };
}

void RayCaster::printInfoPreRendering( void ) const
{
    std::cout << "  # of threads .....................: " << omp_get_max_threads() << std::endl;
    std::cout << "  rendering algorithm ..............: ray casting" << std::endl;
}

void RayCaster::printInfoPostRendering( void ) const
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
                                                          << static_cast< float >( num_intersections ) / num_intersection_tests * 100.0f
                                                          << "% )"
                                                          << std::endl;
}

