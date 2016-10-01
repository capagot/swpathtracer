#ifndef PERSPECTIVE_CAMERA_H_
#define PERSPECTIVE_CAMERA_H_

#include "camera.h"
#include "ray.h"

class PerspectiveCamera : public Camera
{
public:

    PerspectiveCamera( void );

    PerspectiveCamera( const glm::vec3 &position,
                       const glm::vec3 &up_vector,
                       const glm::vec3 &look_at,
                       float aspect,
                       float fov_degrees );

    Ray getWorldSpaceRay( const glm::vec2 &sample_coord ) const;

    void printInfo( void ) const;

    float aspect_;

    float fov_degrees_;

private:

    static const float kDegreesToRadians_;
};

#endif /* PERSPECTIVE_CAMERA_H_ */

