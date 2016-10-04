#ifndef CAMERA_H_
#define CAMERA_H_

#include <iostream>
#include <iomanip>

#include <glm/glm.hpp>
#include "onb.h"
#include "ray.h"

class Camera
{
public:

    Camera( void );

    Camera( const glm::dvec3 &position,
            const glm::dvec3 &up,
            const glm::dvec3 &look_at );

    virtual ~Camera( void );

    void setPosition( const glm::dvec3 &position );

    void setUp( const glm::dvec3 &up );

    void setLookAt( const glm::dvec3 &look_at );

    virtual Ray getWorldSpaceRay( const glm::dvec2 &sample_coord ) const = 0;

    virtual void printInfo( void ) const = 0;

//private:

    glm::dvec3 up_{ 0.0, 1.0, 0.0 };   // up vector (usually equal to universe Y axis: [0, 1, 0]).

    glm::dvec3 look_at_{ 0.0, 0.0, -1.0 };    // point the camera is looking at.

    glm::dvec3 position_{ 0.0, 0.0, 0.0 };    // position of the camera in the universe space.

    glm::dvec3 direction_{ 0.0, 0.0, -1.0 };  // direction the camera is looking.

    ONB onb_;                                   //  orthonormal RHS basis
};

#endif /* CAMERA_H_ */

