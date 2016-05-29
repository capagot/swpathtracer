/*
 * orthographic_camera.cpp
 *
 *  Created on: May 13, 2016
 *      Author: christian
 */

#include "orthographic_camera.h"

OrthographicCamera::OrthographicCamera( unsigned int h_resolution,
                                        unsigned int v_resolution,
                                        const float min_x,
                                        const float max_x,
                                        const float min_y,
                                        const float max_y,
                                        const glm::vec3 &position,
                                        const glm::vec3 &up_vector,
                                        const glm::vec3 &look_at ) :
        Camera::Camera{ h_resolution,
                        v_resolution,
                        position,
                        up_vector,
                        look_at },
        min_x_{ min_x },
        max_x_{ max_x },
        min_y_{ min_y },
        max_y_{ max_y }
{
    aspect_ = ( max_x_ - min_x_ ) / ( max_y_ - min_y_ );

    initTopLeftRay();
}

Ray OrthographicCamera::getRay( unsigned int x, unsigned int y ) const
{
    return Ray{ top_left_pixel_center_ + pixel_width_ * static_cast< float >( x ) + pixel_height_ * static_cast< float >( y ),  // ray origin
                direction_ };  // ray direction
}

void OrthographicCamera::printInfo( void ) const
{
    std::clog << "Orthographic Camera Information" << std::endl;
    std::clog << "-------------------------------" << std::endl;

    Camera::printInfo();

    std::clog << " [ min_x_ , max_x_ ] ...........: [" << min_x_ << ", " << max_x_ << "]" << std::endl;
    std::clog << " [ min_y_ , max_y_ ] ...........: [" << min_y_ << ", " << max_y_ << "]" << std::endl;
    std::clog << " aspect_ .......................: " << aspect_ << std::endl;
    std::clog << " pixel_width_ ..................: " << glm::length( pixel_width_ ) << std::endl;
    std::clog << " pixel_height_ .................: " << glm::length( pixel_height_ ) << std::endl;
}

void OrthographicCamera::initTopLeftRay( void )
{
    glm::vec3 top_left_corner{ onb_.u_ * min_x_ + onb_.v_ * max_y_ + position_ };

    pixel_width_ = ( ( onb_.u_ * max_x_+ onb_.v_ * max_y_+ position_ ) - top_left_corner ) / static_cast< float >( h_resolution_ );
    pixel_height_ = ( ( onb_.u_ * min_x_+ onb_.v_ * min_y_+ position_ ) - top_left_corner ) / static_cast< float >( v_resolution_ );

    top_left_pixel_center_ = top_left_corner + pixel_width_ * 0.5f + pixel_height_ * 0.5f;
}
