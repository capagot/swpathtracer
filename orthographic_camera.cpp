#include "orthographic_camera.h"

OrthographicCamera::OrthographicCamera( void )
{ }

OrthographicCamera::OrthographicCamera( const float min_x,
                                        const float max_x,
                                        const float min_y,
                                        const float max_y,
                                        const glm::vec3 &position,
                                        const glm::vec3 &up_vector,
                                        const glm::vec3 &look_at ) :
        Camera::Camera{ position,
                        up_vector,
                        look_at },
        min_x_{ min_x },
        max_x_{ max_x },
        min_y_{ min_y },
        max_y_{ max_y }
{ }

Ray OrthographicCamera::getWorldSpaceRay( const glm::vec2 &sample_coord ) const
{
    float alpha_x = ( sample_coord.x + 1.0f ) * 0.5f;
    float alpha_y = ( sample_coord.y + 1.0f ) * 0.5f;

    glm::vec3 origin{  ( 1.0f - alpha_x ) * min_x_ + alpha_x * max_x_,
                      -( ( 1.0f - alpha_y ) * min_y_ + alpha_y * max_y_ ),
                       0.0f };

    //return Ray{ onb_.m_ * origin + position_, glm::normalize( onb_.m_* glm::vec3{ 0.0f, 0.0f, -1.0f } ) };
    return Ray{ onb_.getBasisMatrix() * origin + position_, glm::normalize( onb_.getBasisMatrix()* glm::vec3{ 0.0f, 0.0f, -1.0f } ) };
}

void OrthographicCamera::printInfo( void ) const
{
    std::clog << "Orthographic Camera Information" << std::endl;
    std::clog << "-------------------------------" << std::endl;

    std::clog << " [ min_x_ , max_x_ ] ...........: [" << min_x_ << ", " << max_x_ << "]" << std::endl;
    std::clog << " [ min_y_ , max_y_ ] ...........: [" << min_y_ << ", " << max_y_ << "]" << std::endl;
}

