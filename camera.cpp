/*
 * camera.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#include "camera.h"

Camera::Camera( const glm::vec3 &position,
                const glm::vec3 &up_vector,
                const glm::vec3 &look_at ) :
        position_{ position },
        up_vector_{ up_vector },
        look_at_{ look_at },
        direction_{ glm::normalize( look_at - position ) }
{
    // Sets up the camera's ONB.
    onb_.setFromUW( glm::normalize( glm::cross( up_vector_, -direction_ ) ), -direction_ );
}

Camera::~Camera( void )
{ }

void Camera::printInfo( void ) const
{
    std::clog << " position_ .....................: [" << position_[0] << ", "
                                                       << position_[1] << ", "
                                                       << position_[2] << "]" << std::endl;
    std::clog << " up_vector_ ....................: [" << up_vector_[0] << ", "
                                                       << up_vector_[1] << ", "
                                                       << up_vector_[2] << "]" << std::endl;
    std::clog << " direction_ ....................: [" << direction_[0] << ", "
                                                       << direction_[1] << ", "
                                                       << direction_[2] << "]" << std::endl;
}
