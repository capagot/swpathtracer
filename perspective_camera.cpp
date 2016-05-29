/*
 * perspective_camera.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: christian
 */

#include "perspective_camera.h"

// TODO: perhaps it will be better to migrate this constant to the abstract Camera class,
//       if the derived cameras also use it often.
const float PerspectiveCamera::kDegreesToRadians = M_PI / 180.0f;

PerspectiveCamera::PerspectiveCamera( unsigned int h_resolution,
                                      unsigned int v_resolution,
                                      const glm::vec3 &position,
                                      const glm::vec3 &up_vector,
                                      const glm::vec3 &look_at,
                                      float fov_degrees ) :
        Camera::Camera{ h_resolution,
                        v_resolution,
                        position,
                        up_vector,
                        look_at },
        fov_degrees_{ fov_degrees }
{
    aspect_ = static_cast< float >( h_resolution_) / v_resolution_;

    setTopLeftPixelCenter();
}

Ray PerspectiveCamera::getRay( unsigned int x, unsigned int y ) const
{
    // TODO: I think that the computation of the direction can be simplified....
    //       for instance... (top_left_pixel_center - position) can be precomputed and stored.
    return Ray{ position_,
                glm::normalize( ( top_left_pixel_center + pixel_width_ * static_cast< float >( x ) + pixel_height_ * static_cast< float >( y ) ) - position_ )};
}

void PerspectiveCamera::printInfo( void ) const
{
    std::clog << "Perspective Camera Information" << std::endl;
    std::clog << "------------------------------" << std::endl;

    Camera::printInfo();

    std::clog << " fov_degrees_ ..................: " << fov_degrees_ << std::endl;
    std::clog << " aspect_ .......................: " << aspect_ << std::endl;
    std::clog << " pixel_width_ ..................: " << glm::length( pixel_width_ ) << std::endl;
    std::clog << " pixel_height_ .................: " << glm::length( pixel_height_ ) << std::endl;
}

void PerspectiveCamera::setTopLeftPixelCenter( void )
{
    // TODO: this computation might be optimized!

    glm::vec3 screen_center{ -onb_.w_ * static_cast< float >( 1.0f /  tan( fov_degrees_ * 0.5f * PerspectiveCamera::kDegreesToRadians ) ) + position_ };
    glm::vec3 top_left_corner = screen_center - onb_.u_  + ( onb_.v_ * 1.0f / aspect_ );
    glm::vec3 bottom_left_corner = screen_center - onb_.u_  - ( onb_.v_ * 1.0f / aspect_ );
    glm::vec3 top_right_corner = screen_center + onb_.u_  + ( onb_.v_ * 1.0f / aspect_ );

    pixel_width_ = ( top_right_corner - top_left_corner ) / static_cast< float >( h_resolution_ );
    pixel_height_ = ( bottom_left_corner - top_left_corner ) / static_cast< float >( v_resolution_ );

    top_left_pixel_center = top_left_corner + pixel_width_ * 0.5f + pixel_height_ * 0.5f;
}
