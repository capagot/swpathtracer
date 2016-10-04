#include "orthographic_camera.h"

OrthographicCamera::OrthographicCamera( void )
{ }

OrthographicCamera::OrthographicCamera( const double min_x,
                                        const double max_x,
                                        const double min_y,
                                        const double max_y,
                                        const glm::dvec3 &position,
                                        const glm::dvec3 &up_vector,
                                        const glm::dvec3 &look_at ) :
        Camera::Camera{ position,
                        up_vector,
                        look_at },
        min_x_{ min_x },
        max_x_{ max_x },
        min_y_{ min_y },
        max_y_{ max_y }
{ }

Ray OrthographicCamera::getWorldSpaceRay( const glm::dvec2 &sample_coord ) const
{
    double alpha_x = ( sample_coord.x + 1.0 ) * 0.5;
    double alpha_y = ( sample_coord.y + 1.0 ) * 0.5;

    glm::dvec3 origin{  ( 1.0 - alpha_x ) * min_x_ + alpha_x * max_x_,
                      -( ( 1.0 - alpha_y ) * min_y_ + alpha_y * max_y_ ),
                       0.0 };

    //return Ray{ onb_.m_ * origin + position_, glm::normalize( onb_.m_* glm::dvec3{ 0.0, 0.0, -1.0 } ) };
    return Ray{ onb_.getBasisMatrix() * origin + position_, glm::normalize( onb_.getBasisMatrix()* glm::dvec3{ 0.0, 0.0, -1.0 } ) };
}

void OrthographicCamera::printInfo( void ) const
{
    std::clog << "Orthographic Camera Information" << std::endl;
    std::clog << "-------------------------------" << std::endl;

    std::clog << " [ min_x_ , max_x_ ] ...........: [" << min_x_ << ", " << max_x_ << "]" << std::endl;
    std::clog << " [ min_y_ , max_y_ ] ...........: [" << min_y_ << ", " << max_y_ << "]" << std::endl;
}

