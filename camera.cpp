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

