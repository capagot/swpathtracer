#ifndef ORTHOGRAPHIC_CAMERA_H_
#define ORTHOGRAPHIC_CAMERA_H_

#include "camera.h"
#include "ray.h"

class OrthographicCamera : public Camera
{
public:

    OrthographicCamera( void );

    OrthographicCamera( const double min_x,
                        const double max_x,
                        const double min_y,
                        const double max_y,
                        const glm::dvec3 &position,
                        const glm::dvec3 &up_vector,
                        const glm::dvec3 &look_at );

    Ray getWorldSpaceRay( const glm::dvec2 &sample_coord ) const;

    void printInfo( void ) const;

    double min_x_;

    double max_x_;

    double min_y_;

    double max_y_;

};

#endif /* ORTHOGRAPHIC_CAMERA_H_ */

