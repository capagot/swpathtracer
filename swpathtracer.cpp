/*
 * swpathtracer.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#include "swpathtracer.h"

// TODO: put globals into a system-wide class (singleton?).

float min_x = -1.0f;
float max_x =  1.0f;
float min_y = -0.5f;
float max_y =  0.5f;

glm::vec3 position{ 1.0f, 1.0f, 2.0f };
glm::vec3 up_vector{ 0.0f, 1.0f, 0.0f };
glm::vec3 look_at{ 0.0f, 0.0f, 0.0f };

unsigned int image_h_resolution = 1024;
unsigned int image_v_resolution = 1024;
float camera_field_of_view = 90.0f;
int spectrum_num_samples = 3;
Spectrum background_color{ glm::vec3{ 0.0f, 0.0f, 0.2f } };

int main( void )
{
    PerspectiveCamera perspective_camera( image_h_resolution,
                                          image_v_resolution,
                                          position,
                                          up_vector,
                                          look_at,
                                          camera_field_of_view );
    perspective_camera.printInfo();


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

    Scene scene{};
    //scene.load();
    scene.loadFromFile("models/tri_cube.obj");
    //scene.loadFromFile("models/tri_monkey.obj");

    std::clog << std::endl;
    scene.printInfo();

    Buffer rendering_buffer{ image_h_resolution,
                             image_v_resolution,
                             spectrum_num_samples };

    std::clog << std::endl;
    rendering_buffer.printInfo();

    PathTracer pt{ perspective_camera,
                   scene,
                   rendering_buffer,
                   background_color };

    std::clog << std::endl;
    pt.printInfo();

    std::clog << std::endl;
    pt.render();

    std::clog << std::endl;
    rendering_buffer.save( "output.ppm" );

    return EXIT_SUCCESS;
}
