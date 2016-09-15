/*
 * perspective_camera.cpp
 *
 *  Created on: Apr 23, 2016
 *      Author: christian
 */

#include "perspective_camera.h"

const float PerspectiveCamera::kDegreesToRadians = M_PI / 180.0f;

PerspectiveCamera::PerspectiveCamera( const glm::vec3 &position,
                                      const glm::vec3 &up_vector,
                                      const glm::vec3 &look_at,
                                      float aspect,
                                      float fov_degrees ) :
        Camera::Camera{ position,
                        up_vector,
                        look_at },
        fov_degrees_{ fov_degrees }
{ }

Ray PerspectiveCamera::getWorldSpaceRay( const glm::vec2 &sample_coord ) const
{
    glm::vec3 ray_local_dir{  sample_coord.x * aspect_ * tan ( fov_degrees_ * 0.5f * PerspectiveCamera::kDegreesToRadians ),
                             -sample_coord.y * tan ( fov_degrees_ * 0.5f * PerspectiveCamera::kDegreesToRadians ),
                             -1.0f };

    return Ray{ position_, glm::normalize( onb_.m_ * ray_local_dir ) };
}

void PerspectiveCamera::printInfo( void ) const
{
    std::clog << "Perspective Camera Information" << std::endl;
    std::clog << "------------------------------" << std::endl;

    Camera::printInfo();

    std::clog << " fov_degrees_ ..................: " << fov_degrees_ << std::endl;
    std::clog << " aspect_ .......................: " << aspect_ << std::endl;
}
