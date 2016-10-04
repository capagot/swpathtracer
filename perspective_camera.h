#ifndef PERSPECTIVE_CAMERA_H_
#define PERSPECTIVE_CAMERA_H_

#include "camera.h"
#include "ray.h"

class PerspectiveCamera : public Camera
{
public:

    PerspectiveCamera( void );

    PerspectiveCamera( const glm::dvec3 &position,
                       const glm::dvec3 &up_vector,
                       const glm::dvec3 &look_at,
                       double aspect,
                       double fov_degrees );

    Ray getWorldSpaceRay( const glm::dvec2 &sample_coord ) const;

    void printInfo( void ) const;

    double aspect_;

    double fov_degrees_;

private:

    static const double kDegreesToRadians_;
};

#endif /* PERSPECTIVE_CAMERA_H_ */

