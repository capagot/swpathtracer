/*
 * perspectivecamera.h
 *
 *  Created on: Apr 22, 2016
 *      Author: christian
 */

#ifndef PERSPECTIVE_CAMERA_H_
#define PERSPECTIVE_CAMERA_H_

#include "camera.h"
#include "ray.h"

class PerspectiveCamera : public Camera
{
public:

    PerspectiveCamera( unsigned int h_resolution,
                       unsigned int v_resolution,
                       const glm::vec3 &position,
                       const glm::vec3 &up_vector,
                       const glm::vec3 &look_at,
                       float fov_degrees );

    Ray getRay( unsigned int x, unsigned int y ) const;

    void printInfo( void ) const;

    float fov_degrees_ = 55.0f;

private:

    void setTopLeftPixelCenter( void );

    glm::vec3 pixel_width_;

    glm::vec3 pixel_height_;

    glm::vec3 top_left_pixel_center;

    float aspect_; // v_resolution_ / h_resolution_

    static const float kDegreesToRadians;
};

#endif /* PERSPECTIVE_CAMERA_H_ */
