/*
 * camera.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#include "camera.h"

Camera::Camera( void )
{ }

Camera::Camera( const glm::vec3 &position,
                const glm::vec3 &up,
                const glm::vec3 &look_at ) :
        up_{ up },
        look_at_{ look_at },
        position_{ position },
        direction_{ glm::normalize( look_at - position ) }
{
    // Sets up the camera's ONB.
    onb_.setFromUW( glm::normalize( glm::cross( up_, -direction_ ) ), -direction_ );
}

Camera::~Camera( void )
{ }

void Camera::setPosition( const glm::vec3 &position )
{
    position_ = position;
    direction_ = glm::normalize( look_at_ - position_ );
    onb_.setFromUW( glm::normalize( glm::cross( up_, -direction_ ) ), -direction_ );
}

void Camera::setUp( const glm::vec3 &up )
{
    up_ = up;
    onb_.setFromUW( glm::normalize( glm::cross( up_, -direction_ ) ), -direction_ );
}

void Camera::setLookAt( const glm::vec3 &look_at )
{
    look_at_ = look_at;
    direction_ = glm::normalize( look_at_ - position_ );
    onb_.setFromUW( glm::normalize( glm::cross( up_, -direction_ ) ), -direction_ );
}

void Camera::printInfo( void ) const
{
    std::clog << " position_ .....................: [" << position_[0] << ", "
                                                       << position_[1] << ", "
                                                       << position_[2] << "]" << std::endl;
    std::clog << " up_vector_ ....................: [" << up_[0] << ", "
                                                       << up_[1] << ", "
                                                       << up_[2] << "]" << std::endl;
    std::clog << " direction_ ....................: [" << direction_[0] << ", "
                                                       << direction_[1] << ", "
                                                       << direction_[2] << "]" << std::endl;
}
