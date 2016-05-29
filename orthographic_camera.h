/*
 * orthographic_camera.h
 *
 *  Created on: May 13, 2016
 *      Author: christian
 */

#ifndef ORTHOGRAPHIC_CAMERA_H_
#define ORTHOGRAPHIC_CAMERA_H_

#include "camera.h"
#include "ray.h"

class OrthographicCamera : public Camera
{
public:

    OrthographicCamera( unsigned int h_resolution,
                        unsigned int v_resolution,
                        const float min_x,
                        const float max_x,
                        const float min_y,
                        const float max_y,
                        const glm::vec3 &position,
                        const glm::vec3 &up_vector,
                        const glm::vec3 &look_at );

    Ray getRay( unsigned int x, unsigned int y ) const;

    void printInfo( void ) const;

private:

    void initTopLeftRay( void );

    float min_x_;

    float max_x_;

    float min_y_;

    float max_y_;

    float aspect_;

    glm::vec3 top_left_pixel_center_;

    glm::vec3 pixel_width_;

    glm::vec3 pixel_height_;
};

#endif /* ORTHOGRAPHIC_CAMERA_H_ */
