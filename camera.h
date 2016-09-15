/*
 * camera.h
 *
 *  Created on: Apr 10, 2016
 *      Author: christian
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>

#include <glm/glm.hpp>
#include "onb.h"
#include "ray.h"

class Camera
{
public:

    Camera( const glm::vec3 &position,
            const glm::vec3 &up_vector,
            const glm::vec3 &look_at );

    ~Camera( void );

    virtual Ray getWorldSpaceRay( const glm::vec2 &sample_coord ) const = 0;

    void printInfo( void ) const;

    glm::vec3 position_{ 0.0f, 0.0f, 0.0f };    // position of the camera in the universe space.

    glm::vec3 up_vector_{ 0.0f, 1.0f, 0.0f };   // up vector (usually equal to universe Y axis: [0, 1, 0]).

    glm::vec3 look_at_{ 0.0f, 0.0f, -1.0f };    // point the camera is looking at.

    glm::vec3 direction_{ 0.0f, 0.0f, -1.0f };  // direction the camera is looking.

    ONB onb_;                                   //  orthonormal RHS basis
};

#endif /* CAMERA_H_ */
