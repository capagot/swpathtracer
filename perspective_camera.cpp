#include "perspective_camera.h"

const double PerspectiveCamera::kDegreesToRadians_ = M_PI / 180.0;

PerspectiveCamera::PerspectiveCamera( void )
{ }

PerspectiveCamera::PerspectiveCamera( const glm::dvec3 &position,
                                      const glm::dvec3 &up_vector,
                                      const glm::dvec3 &look_at,
                                      double aspect,
                                      double fov_degrees ) :
        Camera::Camera{ position,
                        up_vector,
                        look_at },
        aspect_{ aspect },
        fov_degrees_{ fov_degrees }
{ }

Ray PerspectiveCamera::getWorldSpaceRay( const glm::dvec2 &sample_coord ) const
{
    glm::dvec3 ray_local_dir{  sample_coord.x * aspect_ * tan ( fov_degrees_ * 0.5 * PerspectiveCamera::kDegreesToRadians_ ),
                             -sample_coord.y * tan ( fov_degrees_ * 0.5 * PerspectiveCamera::kDegreesToRadians_ ),
                             -1.0 };

    return Ray{ position_, glm::normalize( onb_.getBasisMatrix() * ray_local_dir ) };
}

void PerspectiveCamera::printInfo( void ) const
{
    std::cout << "Camera: " << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "  type .............................: perspective" << std::endl;
    std::cout << "  position .........................: [" << position_[0] << ", " 
                                                           << position_[1] << ", " 
                                                           << position_[2] << "] " << std::endl;
    std::cout << "  look at ..........................: [" << look_at_[0] << ", " 
                                                           << look_at_[1] << ", " 
                                                           << look_at_[2] << "] " << std::endl;
    std::cout << "  up ...............................: [" << up_[0] << ", " 
                                                           << up_[1] << ", " 
                                                           << up_[2] << "] " << std::endl;
    std::clog << "  aspect ...........................: " << std::fixed << std::setw( 4 )
                                                          << std::setprecision( 2 )
                                                          << aspect_ << std::endl;
    std::clog << "  fov ..............................: " << fov_degrees_ << " degrees" << std::endl;
}

