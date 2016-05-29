/*
 * camera.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#include "camera.h"

Camera::Camera( unsigned int h_resolution,
                unsigned int v_resolution,
                const glm::vec3 &position,
                const glm::vec3 &up_vector,
                const glm::vec3 &look_at ) :
        h_resolution_{ h_resolution },
        v_resolution_{ v_resolution },
        position_{ position },
        up_vector_{ up_vector },
        look_at_{ look_at },
        direction_{ glm::normalize( look_at - position ) }
{
    // updated the camera's RHS orthonormal basis.
    onb_.setUpONB( up_vector_, direction_ );
}

Camera::~Camera( void )
{ }

//void Camera::setHResolution( unsigned int h_resolution )
//{
//    h_resolution_ = h_resolution;
//}

//void Camera::setVResolution( unsigned int v_resolution )
//{
//    v_resolution_ = v_resolution;
//}

//void Camera::setPosition( const glm::vec3 &position )
//{
//    position_ = position;
//}

//void Camera::setDirection( const glm::vec3 &look_at )
//{
//    look_at_ = look_at;
    // updated the camera's RHS orthonormal basis.
//    onb_.setUpONB( up_vector_, look_at_ );
//}

//void Camera::setUpVector( const glm::vec3 &up_vector )
//{
//    up_vector_ = up_vector;
    // updated the camera's RHS orthonormal basis.
//    onb_.setUpONB( up_vector_, direction_ );
//}

void Camera::printInfo( void ) const
{
    std::clog << " h_resolution_ .................: " << h_resolution_ << std::endl;
    std::clog << " v_resolution_ .................: " << v_resolution_ << std::endl;
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
