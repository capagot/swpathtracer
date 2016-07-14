/*
 * perspective_camera.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: christian
 */

#include "perspective_camera.h"

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
    setTopLeftCorner();
}

Ray PerspectiveCamera::getRay( const glm::vec2 &sample_coord ) const
{
    return Ray{ position_,
                glm::normalize( ( top_left_corner_ + pixel_width_ * sample_coord.x + pixel_height_ * sample_coord.y ) - position_ )};
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

void PerspectiveCamera::setTopLeftCorner( void )
{
    glm::vec3 screen_center{ -onb_.w_ * static_cast< float >( 1.0f /  tan( fov_degrees_ * 0.5f * PerspectiveCamera::kDegreesToRadians ) ) + position_ };
    top_left_corner_ = screen_center - onb_.u_  + ( onb_.v_ / aspect_ );
}
