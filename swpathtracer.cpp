/*
 * swpathtracer.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#include "swpathtracer.h"

// TODO: put globals into a system-wide class (singleton?).

glm::vec3 position{ 0.0f, 0.0f, 6.0f };
glm::vec3 up_vector{ 0.0f, 1.0f, 0.0f };
glm::vec3 look_at{ 0.0f, 0.0f, 0.0f };

unsigned int image_h_resolution = 512;
unsigned int image_v_resolution = 512;
float camera_field_of_view = 55.0f;
int spectrum_num_samples = 3;
Spectrum background_color{ glm::vec3{ 0.0f, 0.0f, 0.2f } };

int main( void )
{
    PerspectiveCamera camera( image_h_resolution,
                              image_v_resolution,
                              position,
                              up_vector,
                              look_at,
                              camera_field_of_view );
    camera.printInfo();


    /*
    OrthographicCamera orthographic_camera( image_h_resolution,
                                            image_v_resolution,
                                            min_x,
                                            max_x,
                                            min_y,
                                            max_y,
                                            position,
                                            up_vector,
                                            look_at );
    std::clog << std::endl;
    orthographic_camera.printInfo();
    //*/

    // TODO: put min and max into scene
    glm::vec3 min;
    glm::vec3 max;
    Scene scene{};
    scene.load();
    //scene.loadFromFile( "models/tri_cube.obj", min, max );
    scene.loadFromFile( "models/tri_monkey.obj", min, max );
    //std::clog << "model aabb: [" << min.x << ", "
    //                             << min.y << ", "
    //                             << min.z << "] - ["
    //                             << max.x << ", "
    //                             << max.y << ", "
    //                             << max.z << "]" << std::endl;

    std::clog << std::endl;
    scene.printInfo();

    Buffer rendering_buffer{ image_h_resolution,
                             image_v_resolution,
                             spectrum_num_samples };

    std::clog << std::endl;
    rendering_buffer.printInfo();

    //only one instance of each PRNG engine is be kept during execution.
    RNG< std::uniform_real_distribution, float, std::mt19937 > rng{ 0.0f, 1.0f };

    //UniformSampler sampler( rng, 16 );
    //RegularSampler sampler( 1 );
    JitteredSampler sampler( rng, 16 );

    PathTracer pt( camera,
                   scene,
                   background_color,
                   5,
                   Integrator:: MAX_DEPTH,
                   sampler,
                   rendering_buffer );

    pt.integrate();

    //RenderingEngine re( camera,
    //                    scene,
    //                    pt,
    //                    sampler,
    //                    rendering_buffer );
    //re.renderImage();

    rendering_buffer.save( "output.ppm" );


    return EXIT_SUCCESS;
}
